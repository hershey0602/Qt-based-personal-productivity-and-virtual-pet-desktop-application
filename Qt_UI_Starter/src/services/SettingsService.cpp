#include "SettingsService.h"
#include "mocks/MockPorts.h"
namespace fg {
SettingsService::SettingsService(const QString& settingsFilePath, QObject* parent) : ISettingsService(parent) {}
OperationResult SettingsService::load() { return {false, notImplemented()}; }
Preferences SettingsService::preferences() const { return {}; }
OperationResult SettingsService::savePreferences(const Preferences& preferences) { return {false, notImplemented()}; }
MusicPreferences SettingsService::musicPreferences() const { return {}; }
OperationResult SettingsService::saveMusicPreferences(const MusicPreferences& preferences) { return {false, notImplemented()}; }
}
