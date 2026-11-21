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
};
}
