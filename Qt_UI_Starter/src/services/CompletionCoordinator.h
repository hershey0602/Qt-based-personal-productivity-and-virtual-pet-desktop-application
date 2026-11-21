#pragma once
#include "contracts/ServicePorts.h"
namespace fg { class CompletionCoordinator : public QObject {
 Q_OBJECT
public:
 CompletionCoordinator(IFocusEngine& focus, IRewardService& rewards, IRepository& repository, QObject* parent=nullptr);
public slots:
void settle(fg::SessionId sessionId);
OperationResult retry(const SessionId& sessionId);
OperationResult retryPending();
signals:
void settlementCompleted(fg::SettlementReceipt receipt);
void settlementFailed(fg::SessionId sessionId, fg::AppError error);
private:
 IFocusEngine& focus_; IRewardService& rewards_; IRepository& repository_;
}; }
