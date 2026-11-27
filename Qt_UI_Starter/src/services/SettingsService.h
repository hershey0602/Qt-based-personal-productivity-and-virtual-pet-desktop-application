#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class SettingsService : public ISettingsService {
 Q_OBJECT
public:
 explicit SettingsService(const QString& settingsFilePath, QObject* parent=nullptr);
OperationResult load() override;
Preferences preferences() const override;
OperationResult savePreferences(const Preferences& preferences) override;
MusicPreferences musicPreferences() const override;
OperationResult saveMusicPreferences(const MusicPreferences& preferences) override;
private:
 QString settingsPath_;
 Preferences preferences_{};
 MusicPreferences musicPreferences_{};
 bool loaded_=false;

 static bool validPreferences(const Preferences& preferences);
 static bool validMusicPreferences(const MusicPreferences& preferences);
 static AppError validationError(const QString& message);
 OperationResult persist(const Preferences* preferences,
                         const MusicPreferences* musicPreferences);
};
}
