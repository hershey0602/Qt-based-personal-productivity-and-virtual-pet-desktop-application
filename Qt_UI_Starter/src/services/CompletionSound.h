#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class CompletionSound : public ICompletionSound {
 Q_OBJECT
public:
 explicit CompletionSound(ISettingsService& settings, QObject* parent=nullptr);
void playForCompletedSession(const SessionId& sessionId) override;
};
}
