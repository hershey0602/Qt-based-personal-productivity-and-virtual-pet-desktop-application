#pragma once
#include "contracts/ServicePorts.h"
#include <QSoundEffect>
namespace fg {
class CompletionSound : public ICompletionSound {
 Q_OBJECT
public:
 explicit CompletionSound(ISettingsService& settings, QObject* parent=nullptr);
void playForCompletedSession(const SessionId& sessionId) override;
private:
 ISettingsService& settings_;
 QSoundEffect effect_;
 QSet<SessionId> playedSessions_;
};
}
