#include "MusicController.h"

#include <QFileInfo>
#include <QUrl>

namespace fg {
namespace {
AppError musicError(AppErrorCode code, const QString& message, const QString& diagnostic = {})
{
    return {code, message, diagnostic};
}

QString trackName(const QString& path)
{
    return QFileInfo(path).fileName();
}
}

MusicController::MusicController(ISettingsService& settings, QObject* parent)
    : IMusicController(parent), settings_(settings), player_(this), audioOutput_(this)
{
    player_.setAudioOutput(&audioOutput_);
    connect(&player_, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        snapshot_.positionMs = position;
        emitSnapshot();
    });
    connect(&player_, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        snapshot_.durationMs = duration;
        snapshot_.seekable = player_.isSeekable();
        emitSnapshot();
    });
    connect(&player_, &QMediaPlayer::seekableChanged, this, [this](bool seekable) {
        snapshot_.seekable = seekable;
        emitSnapshot();
    });
    connect(&player_, &QMediaPlayer::playbackStateChanged, this,
            [this](QMediaPlayer::PlaybackState state) {
        snapshot_.state = state == QMediaPlayer::PlayingState ? PlaybackState::Playing
                            : state == QMediaPlayer::PausedState ? PlaybackState::Paused
                                                                  : PlaybackState::Stopped;
        emitSnapshot();
    });
    connect(&player_, &QMediaPlayer::mediaStatusChanged, this, &MusicController::handleMediaStatus);
    connect(&player_, &QMediaPlayer::errorOccurred, this, &MusicController::handleMediaError);
}

void MusicController::emitSnapshot()
{
    snapshot_.volume = preferences_.volume;
    emit musicChanged(snapshot_);
}

bool MusicController::validIndex(int index) const
{
    return index >= 0 && index < snapshot_.tracks.size();
}

bool MusicController::validTrack(int index) const
{
    if (!validIndex(index)) {
        return false;
    }
    const QFileInfo info(snapshot_.tracks.at(index).absolutePath);
    return snapshot_.tracks.at(index).available && info.exists() && info.isFile();
}

int MusicController::findNextValid(int start, int direction) const
{
    if (snapshot_.tracks.isEmpty()) {
        return -1;
    }
    int index = start;
    for (int count = 0; count < snapshot_.tracks.size(); ++count) {
        index = (index + direction + snapshot_.tracks.size()) % snapshot_.tracks.size();
        if (validTrack(index)) {
            return index;
        }
    }
    return -1;
}

OperationResult MusicController::noTracksError() const
{
    return {false, musicError(AppErrorCode::FileMissing,
                              QStringLiteral("播放列表中没有可用的本地音频文件"),
                              QStringLiteral("No available local tracks"))};
}

OperationResult MusicController::invalidIndexError() const
{
    return {false, musicError(AppErrorCode::NotFound, QStringLiteral("没有找到所选音乐"),
                              QStringLiteral("Music index is outside the playlist"))};
}

OperationResult MusicController::missingTrackError(const QString& path) const
{
    return {false, musicError(AppErrorCode::FileMissing,
                              QStringLiteral("音频文件不可用：%1").arg(trackName(path)),
                              QStringLiteral("Missing or unreadable local file: %1").arg(path))};
}

OperationResult MusicController::loadPlaylist()
{
    preferences_ = settings_.musicPreferences();
    snapshot_ = MusicSnapshot{};
    snapshot_.volume = qBound(0, preferences_.volume, 100);
    audioOutput_.setVolume(snapshot_.volume / 100.0);

    QSet<QString> seen;
    for (const QString& path : preferences_.localPaths) {
        const QString absolute = QFileInfo(path).absoluteFilePath();
        if (absolute.isEmpty() || seen.contains(absolute)) {
            continue;
        }
        seen.insert(absolute);
        const QFileInfo info(absolute);
        snapshot_.tracks.push_back({absolute, trackName(absolute), info.exists() && info.isFile()});
    }
    playlistLoaded_ = true;
    emitSnapshot();
    return {true, {}};
}

MusicSnapshot MusicController::snapshot() const
{
    return snapshot_;
}

OperationResult MusicController::importFiles(const QStringList& absolutePaths)
{
    if (absolutePaths.isEmpty()) {
        return {false, musicError(AppErrorCode::Validation, QStringLiteral("请选择至少一个本地音频文件"),
                                  QStringLiteral("importFiles received an empty list"))};
    }

    MusicPreferences candidate = preferences_;
    QSet<QString> seen;
    for (const QString& path : candidate.localPaths) {
        seen.insert(QFileInfo(path).absoluteFilePath());
    }
    QStringList invalid;
    for (const QString& path : absolutePaths) {
        const QFileInfo info(path);
        if (!info.isAbsolute() || !info.exists() || !info.isFile()) {
            invalid << path;
            continue;
        }
        const QString absolute = info.absoluteFilePath();
        if (!seen.contains(absolute)) {
            candidate.localPaths << absolute;
            seen.insert(absolute);
        }
    }
    if (!invalid.isEmpty()) {
        return {false, musicError(AppErrorCode::FileMissing,
                                  QStringLiteral("有音频文件不存在或无法读取，请检查后重试"),
                                  QStringLiteral("Invalid import paths: %1").arg(invalid.join(';')))};
    }
    if (candidate.localPaths == preferences_.localPaths) {
        return {true, {}};
    }
    const auto saved = settings_.saveMusicPreferences(candidate);
    if (!saved.ok) {
        return saved;
    }
    preferences_ = candidate;
    return loadPlaylist();
}

OperationResult MusicController::selectInternal(int index, bool resumePlayback)
{
    if (!validIndex(index)) {
        return invalidIndexError();
    }
    if (!validTrack(index)) {
        snapshot_.tracks[index].available = false;
        emitSnapshot();
        return missingTrackError(snapshot_.tracks[index].absolutePath);
    }

    player_.stop();
    player_.setSource(QUrl::fromLocalFile(snapshot_.tracks[index].absolutePath));
    snapshot_.currentIndex = index;
    snapshot_.state = PlaybackState::Stopped;
    snapshot_.positionMs = 0;
    snapshot_.durationMs = 0;
    snapshot_.seekable = false;
    emitSnapshot();
    if (resumePlayback) {
        player_.play();
    }
    return {true, {}};
}

OperationResult MusicController::select(int index)
{
    return selectInternal(index, false);
}

OperationResult MusicController::playPause()
{
    if (!snapshot_.currentIndex.has_value()) {
        const int first = findNextValid(-1, 1);
        if (first < 0) {
            return noTracksError();
        }
        return selectInternal(first, true);
    }
    const int index = *snapshot_.currentIndex;
    if (!validTrack(index)) {
        snapshot_.tracks[index].available = false;
        player_.stop();
        emitSnapshot();
        return missingTrackError(snapshot_.tracks[index].absolutePath);
    }
    if (player_.playbackState() == QMediaPlayer::PlayingState) {
        player_.pause();
    } else {
        player_.play();
    }
    return {true, {}};
}

OperationResult MusicController::stop()
{
    player_.stop();
    player_.setPosition(0);
    snapshot_.state = PlaybackState::Stopped;
    snapshot_.positionMs = 0;
    emitSnapshot();
    return {true, {}};
}

OperationResult MusicController::previous()
{
    if (snapshot_.tracks.isEmpty()) {
        return noTracksError();
    }
    const int current = snapshot_.currentIndex.value_or(0);
    const int nextIndex = findNextValid(current, -1);
    if (nextIndex < 0) {
        player_.stop();
        snapshot_.state = PlaybackState::Stopped;
        emitSnapshot();
        return noTracksError();
    }
    return selectInternal(nextIndex, player_.playbackState() == QMediaPlayer::PlayingState);
}

OperationResult MusicController::next()
{
    if (snapshot_.tracks.isEmpty()) {
        return noTracksError();
    }
    const int current = snapshot_.currentIndex.value_or(-1);
    const int nextIndex = findNextValid(current, 1);
    if (nextIndex < 0) {
        player_.stop();
        snapshot_.state = PlaybackState::Stopped;
        emitSnapshot();
        return noTracksError();
    }
    return selectInternal(nextIndex, player_.playbackState() == QMediaPlayer::PlayingState);
}

OperationResult MusicController::seek(qint64 positionMs)
{
    if (!snapshot_.currentIndex.has_value() || !validTrack(*snapshot_.currentIndex)) {
        return noTracksError();
    }
    if (!player_.isSeekable()) {
        return {false, musicError(AppErrorCode::NotSeekable, QStringLiteral("当前音频暂不支持拖动进度"),
                                  QStringLiteral("QMediaPlayer reports isSeekable=false"))};
    }
    player_.setPosition(qBound<qint64>(0, positionMs, qMax<qint64>(0, player_.duration())));
    return {true, {}};
}

OperationResult MusicController::setVolume(int volume)
{
    if (volume < 0 || volume > 100) {
        return {false, musicError(AppErrorCode::Validation, QStringLiteral("音量必须在0到100之间"),
                                  QStringLiteral("Invalid volume: %1").arg(volume))};
    }
    MusicPreferences candidate = preferences_;
    candidate.volume = volume;
    const auto saved = settings_.saveMusicPreferences(candidate);
    if (!saved.ok) {
        return saved;
    }
    preferences_ = candidate;
    audioOutput_.setVolume(volume / 100.0);
    snapshot_.volume = volume;
    emitSnapshot();
    return {true, {}};
}

OperationResult MusicController::remove(int index)
{
    if (!validIndex(index)) {
        return invalidIndexError();
    }
    const bool wasCurrent = snapshot_.currentIndex.has_value() && *snapshot_.currentIndex == index;
    const bool wasPlaying = player_.playbackState() == QMediaPlayer::PlayingState;
    MusicPreferences candidate = preferences_;
    candidate.localPaths.removeAll(snapshot_.tracks.at(index).absolutePath);
    const auto saved = settings_.saveMusicPreferences(candidate);
    if (!saved.ok) {
        return saved;
    }
    if (wasCurrent) {
        player_.stop();
    }
    preferences_ = candidate;
    snapshot_.tracks.removeAt(index);
    if (snapshot_.tracks.isEmpty()) {
        snapshot_.currentIndex.reset();
        snapshot_.state = PlaybackState::Stopped;
        snapshot_.positionMs = 0;
        snapshot_.durationMs = 0;
        snapshot_.seekable = false;
    } else if (wasCurrent) {
        const int replacement = qMin(index, snapshot_.tracks.size() - 1);
        snapshot_.currentIndex.reset();
        snapshot_.state = PlaybackState::Stopped;
        snapshot_.positionMs = 0;
        snapshot_.durationMs = 0;
        snapshot_.seekable = false;
        const int validReplacement = validTrack(replacement) ? replacement : findNextValid(replacement, 1);
        if (validReplacement >= 0) {
            selectInternal(validReplacement, wasPlaying);
        }
    } else if (snapshot_.currentIndex.has_value() && *snapshot_.currentIndex > index) {
        --*snapshot_.currentIndex;
    }
    emitSnapshot();
    return {true, {}};
}

void MusicController::handleMediaStatus(QMediaPlayer::MediaStatus status)
{
    if (status != QMediaPlayer::EndOfMedia) {
        return;
    }
    const int nextIndex = findNextValid(snapshot_.currentIndex.value_or(-1), 1);
    if (nextIndex < 0) {
        player_.stop();
        snapshot_.state = PlaybackState::Stopped;
        snapshot_.positionMs = 0;
        emitSnapshot();
        return;
    }
    selectInternal(nextIndex, true);
}

void MusicController::handleMediaError(QMediaPlayer::Error mediaError, const QString& errorString)
{
    Q_UNUSED(mediaError)
    if (handlingMediaError_) {
        return;
    }
    handlingMediaError_ = true;
    if (snapshot_.currentIndex.has_value() && validIndex(*snapshot_.currentIndex)) {
        const QString path = snapshot_.tracks.at(*snapshot_.currentIndex).absolutePath;
        snapshot_.tracks[*snapshot_.currentIndex].available = false;
        player_.stop();
        snapshot_.state = PlaybackState::Stopped;
        snapshot_.positionMs = 0;
        emitSnapshot();
        emit errorOccurred(musicError(AppErrorCode::MediaUnsupported,
                                      QStringLiteral("无法播放该音频文件：%1").arg(trackName(path)),
                                      errorString.isEmpty() ? path : errorString));
    }
    handlingMediaError_ = false;
}
}
