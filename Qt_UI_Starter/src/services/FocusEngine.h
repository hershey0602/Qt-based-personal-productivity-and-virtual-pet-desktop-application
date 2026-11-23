#pragma once
#include "contracts/ServicePorts.h"

#include <QElapsedTimer>
#include <QTimer>

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
private slots:
void onTick();
private:
IRepository& repository_;
ISettingsService& settings_;
QTimer timer_;
QElapsedTimer elapsed_;
FocusSnapshot snapshot_;
std::optional<CompletionResult> lastCompletion_;
CompletionWrite pendingCompletion_;
int accumulatedMs_=0;
bool ticking_=false;
int activeSeconds() const;
void publish();
void stopTimer();
OperationResult finishNaturally();
};
}
