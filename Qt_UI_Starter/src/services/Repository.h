#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class Repository : public IRepository {
 Q_OBJECT
public:
 explicit Repository(QObject* parent=nullptr);
OperationResult open(const QString& databasePath) override;
OperationResult close() override;
Result<QVector<Task>> listTasks(const TaskFilter& filter) const override;
Result<std::optional<Task>> task(TaskId id) const override;
Result<Task> createTask(const TaskDraft& draft) override;
Result<Task> updateTask(TaskId id, const TaskDraft& draft) override;
OperationResult setTaskDone(TaskId id, bool completed) override;
OperationResult softDeleteTask(TaskId id) override;
Result<QVector<Task>> todayTasks(const QDate& date, int limit) const override;
Result<TaskCounts> taskCounts() const override;
OperationResult insertRunningSession(const FocusSession& session) override;
OperationResult updateSessionActivity(const SessionId& id, SessionStatus status, int elapsedSeconds, const QDateTime& updatedAtUtc) override;
OperationResult completeSession(const CompletionWrite& write) override;
OperationResult abandonSession(const AbandonWrite& write) override;
Result<std::optional<FocusSession>> session(const SessionId& id) const override;
Result<QVector<FocusSession>> pendingCompletions() const override;
OperationResult recoverInterruptedSessions(const QDateTime& recoveredAtUtc) override;
Result<StatisticsSnapshot> statistics(const QDate& anchorDate) const override;
Result<PetSnapshot> petSnapshot() const override;
Result<SettlementReceipt> creditRewardAtomically(const RewardCreditRequest& request) override;
Result<FeedReceipt> feedPetAtomically(const FeedCommand& command) override;
};
}
