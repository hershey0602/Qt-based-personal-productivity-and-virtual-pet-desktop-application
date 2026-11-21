#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class RewardService : public IRewardService {
 Q_OBJECT
public:
 explicit RewardService(IRepository& repository, QObject* parent=nullptr);
Result<SettlementReceipt> settleCompletedSession(const SessionId& sessionId) override;
};
}
