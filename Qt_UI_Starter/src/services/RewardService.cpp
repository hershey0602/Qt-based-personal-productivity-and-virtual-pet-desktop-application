#include "RewardService.h"
#include "mocks/MockPorts.h"
namespace fg {
RewardService::RewardService(IRepository& repository, QObject* parent) : IRewardService(parent) {}
Result<SettlementReceipt> RewardService::settleCompletedSession(const SessionId& sessionId) { return {false, {}, notImplemented()}; }
}
