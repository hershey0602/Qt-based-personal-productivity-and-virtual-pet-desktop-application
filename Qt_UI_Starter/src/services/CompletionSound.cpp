#include "CompletionSound.h"
#include "mocks/MockPorts.h"
namespace fg {
CompletionSound::CompletionSound(ISettingsService& settings, QObject* parent) : ICompletionSound(parent) {}
void CompletionSound::playForCompletedSession(const SessionId& sessionId) {  }
}
