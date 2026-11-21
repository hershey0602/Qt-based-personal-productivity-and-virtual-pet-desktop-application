#include "FocusEngine.h"
#include "mocks/MockPorts.h"
namespace fg {
FocusEngine::FocusEngine(IRepository& repository, ISettingsService& settings, QObject* parent) : IFocusEngine(parent) {}
FocusSnapshot FocusEngine::snapshot() const { return {}; }
std::optional<CompletionResult> FocusEngine::lastCompletion() const { return {}; }
OperationResult FocusEngine::configure(const FocusConfig& config) { return {false, notImplemented()}; }
OperationResult FocusEngine::prepareNextRound(FocusMode mode) { return {false, notImplemented()}; }
Result<SessionId> FocusEngine::start() { return {false, {}, notImplemented()}; }
OperationResult FocusEngine::pause(const SessionId& expectedSessionId) { return {false, notImplemented()}; }
OperationResult FocusEngine::resume(const SessionId& expectedSessionId) { return {false, notImplemented()}; }
OperationResult FocusEngine::abandon(const SessionId& expectedSessionId) { return {false, notImplemented()}; }
OperationResult FocusEngine::retryCompletionSave(const SessionId& expectedSessionId) { return {false, notImplemented()}; }
void FocusEngine::applySettlementSuccess(const SettlementReceipt& receipt) {  }
void FocusEngine::applySettlementFailure(const SessionId& id, const AppError& error) {  }
void FocusEngine::onPreferencesChanged(fg::Preferences preferences) {  }
}
