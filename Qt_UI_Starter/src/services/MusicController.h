#pragma once
#include "contracts/ServicePorts.h"
#include <QAudioOutput>
#include <QMediaPlayer>
namespace fg {
class MusicController : public IMusicController {
 Q_OBJECT
public:
 explicit MusicController(ISettingsService& settings, QObject* parent=nullptr);
OperationResult loadPlaylist() override;
MusicSnapshot snapshot() const override;
OperationResult importFiles(const QStringList& absolutePaths) override;
OperationResult select(int index) override;
OperationResult playPause() override;
OperationResult stop() override;
OperationResult previous() override;
OperationResult next() override;
OperationResult seek(qint64 positionMs) override;
OperationResult setVolume(int volume) override;
OperationResult remove(int index) override;
private:
 ISettingsService& settings_;
 QMediaPlayer player_;
 QAudioOutput audioOutput_;
 MusicPreferences preferences_{};
 MusicSnapshot snapshot_{};
 bool playlistLoaded_=false;
 bool handlingMediaError_=false;

 void emitSnapshot();
 void connectPlayerSignals();
 bool validIndex(int index) const;
 bool validTrack(int index) const;
 int findNextValid(int start, int direction) const;
 OperationResult selectInternal(int index, bool resumePlayback);
 OperationResult noTracksError() const;
 OperationResult invalidIndexError() const;
 OperationResult missingTrackError(const QString& path) const;
 void handleMediaError(QMediaPlayer::Error error, const QString& errorString);
 void handleMediaStatus(QMediaPlayer::MediaStatus status);
};
}
