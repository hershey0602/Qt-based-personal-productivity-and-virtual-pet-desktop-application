#pragma once
#include "contracts/ServicePorts.h"
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
};
}
