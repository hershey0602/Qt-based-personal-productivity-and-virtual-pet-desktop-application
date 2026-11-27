#include "SettingsService.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

namespace fg {
namespace {
constexpr auto kFocusMinutesKey = "preferences/defaultFocusMinutes";
constexpr auto kRestMinutesKey = "preferences/defaultRestMinutes";
constexpr auto kSoundEnabledKey = "preferences/completionSoundEnabled";
constexpr auto kMusicPathsKey = "music/localPaths";
constexpr auto kMusicVolumeKey = "music/volume";

AppError persistenceError(const QString& diagnostic)
{
    return {AppErrorCode::Persistence,
            QStringLiteral("本地设置保存失败，请检查应用数据目录权限后重试"), diagnostic};
}

QString resolveSettingsPath(const QString& requested)
{
    QString path = requested.trimmed();
    if (path.isEmpty()) {
        path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    const QFileInfo info(path);
    if (info.suffix().compare(QStringLiteral("ini"), Qt::CaseInsensitive) == 0) {
        return info.absoluteFilePath();
    }
    return QDir(path).filePath(QStringLiteral("app.ini"));
}

bool isAbsolutePathList(const QStringList& paths)
{
    for (const QString& path : paths) {
        if (path.isEmpty() || !QFileInfo(path).isAbsolute()) {
            return false;
        }
    }
    return true;
}
}

SettingsService::SettingsService(const QString& settingsFilePath, QObject* parent)
    : ISettingsService(parent), settingsPath_(resolveSettingsPath(settingsFilePath))
{
}

bool SettingsService::validPreferences(const Preferences& preferences)
{
    return (preferences.defaultFocusMinutes == 25 || preferences.defaultFocusMinutes == 50)
        && (preferences.defaultRestMinutes == 5 || preferences.defaultRestMinutes == 10);
}

bool SettingsService::validMusicPreferences(const MusicPreferences& preferences)
{
    return preferences.volume >= 0 && preferences.volume <= 100
        && isAbsolutePathList(preferences.localPaths);
}

AppError SettingsService::validationError(const QString& message)
{
    return {AppErrorCode::Validation, message, QStringLiteral("SettingsService validation failed")};
}

OperationResult SettingsService::load()
{
    QSettings settings(settingsPath_, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError) {
        return {false, persistenceError(QStringLiteral("读取设置文件失败: %1").arg(settings.fileName()))};
    }

    Preferences loadedPreferences;
    MusicPreferences loadedMusic;
    const bool hasPreferences = settings.contains(kFocusMinutesKey)
        || settings.contains(kRestMinutesKey) || settings.contains(kSoundEnabledKey);
    const bool hasMusic = settings.contains(kMusicPathsKey) || settings.contains(kMusicVolumeKey);

    if (hasPreferences) {
        loadedPreferences.defaultFocusMinutes = settings.value(kFocusMinutesKey, 25).toInt();
        loadedPreferences.defaultRestMinutes = settings.value(kRestMinutesKey, 5).toInt();
        loadedPreferences.completionSoundEnabled = settings.value(kSoundEnabledKey, true).toBool();
        if (!validPreferences(loadedPreferences)) {
            preferences_ = Preferences{};
            musicPreferences_ = MusicPreferences{};
            loaded_ = false;
            return {false, validationError(QStringLiteral("设置文件中的专注或休息时长无效，已恢复默认值"))};
        }
    }

    if (hasMusic) {
        loadedMusic.localPaths = settings.value(kMusicPathsKey, QStringList{}).toStringList();
        loadedMusic.volume = settings.value(kMusicVolumeKey, 70).toInt();
        if (!validMusicPreferences(loadedMusic)) {
            preferences_ = Preferences{};
            musicPreferences_ = MusicPreferences{};
            loaded_ = false;
            return {false, validationError(QStringLiteral("音乐设置文件中的音量或路径无效，已恢复默认值"))};
        }
    }

    preferences_ = loadedPreferences;
    musicPreferences_ = loadedMusic;
    loaded_ = true;
    return {true, {}};
}

Preferences SettingsService::preferences() const
{
    return preferences_;
}

OperationResult SettingsService::persist(const Preferences* preferences,
                                         const MusicPreferences* musicPreferences)
{
    const Preferences oldPreferences = preferences_;
    const MusicPreferences oldMusicPreferences = musicPreferences_;

    QDir().mkpath(QFileInfo(settingsPath_).absolutePath());
    QSettings settings(settingsPath_, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError) {
        return {false, persistenceError(QStringLiteral("打开设置文件失败: %1").arg(settings.fileName()))};
    }
    if (preferences) {
        settings.setValue(kFocusMinutesKey, preferences->defaultFocusMinutes);
        settings.setValue(kRestMinutesKey, preferences->defaultRestMinutes);
        settings.setValue(kSoundEnabledKey, preferences->completionSoundEnabled);
    }
    if (musicPreferences) {
        settings.setValue(kMusicPathsKey, musicPreferences->localPaths);
        settings.setValue(kMusicVolumeKey, musicPreferences->volume);
    }
    settings.sync();
    if (settings.status() != QSettings::NoError) {
        if (preferences) {
            settings.setValue(kFocusMinutesKey, oldPreferences.defaultFocusMinutes);
            settings.setValue(kRestMinutesKey, oldPreferences.defaultRestMinutes);
            settings.setValue(kSoundEnabledKey, oldPreferences.completionSoundEnabled);
        }
        if (musicPreferences) {
            settings.setValue(kMusicPathsKey, oldMusicPreferences.localPaths);
            settings.setValue(kMusicVolumeKey, oldMusicPreferences.volume);
        }
        settings.sync();
        return {false, persistenceError(QStringLiteral("写入设置文件失败: %1").arg(settings.fileName()))};
    }

    if (preferences) {
        preferences_ = *preferences;
        emit preferencesChanged(preferences_);
    }
    if (musicPreferences) {
        musicPreferences_ = *musicPreferences;
        emit musicPreferencesChanged(musicPreferences_);
    }
    loaded_ = true;
    return {true, {}};
}

OperationResult SettingsService::savePreferences(const Preferences& preferences)
{
    if (!validPreferences(preferences)) {
        return {false, validationError(QStringLiteral("专注时长只能是25或50分钟，休息时长只能是5或10分钟"))};
    }
    return persist(&preferences, nullptr);
}

MusicPreferences SettingsService::musicPreferences() const
{
    return musicPreferences_;
}

OperationResult SettingsService::saveMusicPreferences(const MusicPreferences& preferences)
{
    if (!validMusicPreferences(preferences)) {
        return {false, validationError(QStringLiteral("音乐路径必须是绝对路径，音量必须在0到100之间"))};
    }
    return persist(nullptr, &preferences);
}
}
