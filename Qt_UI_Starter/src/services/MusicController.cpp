#include "MusicController.h"
#include "mocks/MockPorts.h"
namespace fg {
MusicController::MusicController(ISettingsService& settings, QObject* parent) : IMusicController(parent) {}
OperationResult MusicController::loadPlaylist() { return {false, notImplemented()}; }
MusicSnapshot MusicController::snapshot() const { return {}; }
OperationResult MusicController::importFiles(const QStringList& absolutePaths) { return {false, notImplemented()}; }
OperationResult MusicController::select(int index) { return {false, notImplemented()}; }
OperationResult MusicController::playPause() { return {false, notImplemented()}; }
OperationResult MusicController::stop() { return {false, notImplemented()}; }
OperationResult MusicController::previous() { return {false, notImplemented()}; }
OperationResult MusicController::next() { return {false, notImplemented()}; }
OperationResult MusicController::seek(qint64 positionMs) { return {false, notImplemented()}; }
OperationResult MusicController::setVolume(int volume) { return {false, notImplemented()}; }
OperationResult MusicController::remove(int index) { return {false, notImplemented()}; }
}
