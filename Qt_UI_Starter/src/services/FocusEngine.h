#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class FocusEngine : public IFocusEngine {
 Q_OBJECT
public:
 explicit FocusEngine(IRepository& repository, ISettingsService& settings, QObject* parent=nullptr);
FocusSnapshot snapshot() const override;
std::optional<CompletionResult> lastCompletion() const override;
OperationResult configure(const FocusConfig& config) override;
OperationResult prepareNextRound(FocusMode mode) override;
Result<SessionId> start() override;
OperationResult pause(const SessionId& expectedSessionId) override;
OperationResult resume(const SessionId& expectedSessionId) override;
OperationResult abandon(const SessionId& expectedSessionId) override;
OperationResult retryCompletionSave(const SessionId& expectedSessionId) override;
void applySettlementSuccess(const SettlementReceipt& receipt) override;
void applySettlementFailure(const SessionId& id, const AppError& error) override;
public slots:
void onPreferencesChanged(fg::Preferences preferences) override;
};
}
