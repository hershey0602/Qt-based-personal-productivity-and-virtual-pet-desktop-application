#include "Repository.h"
#include "mocks/MockPorts.h"
namespace fg {
Repository::Repository(QObject* parent) : IRepository(parent) {}
OperationResult Repository::open(const QString& databasePath) { return {false, notImplemented()}; }
OperationResult Repository::close() { return {false, notImplemented()}; }
Result<QVector<Task>> Repository::listTasks(const TaskFilter& filter) const { return {false, {}, notImplemented()}; }
Result<std::optional<Task>> Repository::task(TaskId id) const { return {false, {}, notImplemented()}; }
Result<Task> Repository::createTask(const TaskDraft& draft) { return {false, {}, notImplemented()}; }
Result<Task> Repository::updateTask(TaskId id, const TaskDraft& draft) { return {false, {}, notImplemented()}; }
OperationResult Repository::setTaskDone(TaskId id, bool completed) { return {false, notImplemented()}; }
OperationResult Repository::softDeleteTask(TaskId id) { return {false, notImplemented()}; }
Result<QVector<Task>> Repository::todayTasks(const QDate& date, int limit) const { return {false, {}, notImplemented()}; }
Result<TaskCounts> Repository::taskCounts() const { return {false, {}, notImplemented()}; }
OperationResult Repository::insertRunningSession(const FocusSession& session) { return {false, notImplemented()}; }
OperationResult Repository::updateSessionActivity(const SessionId& id, SessionStatus status, int elapsedSeconds, const QDateTime& updatedAtUtc) { return {false, notImplemented()}; }
OperationResult Repository::completeSession(const CompletionWrite& write) { return {false, notImplemented()}; }
OperationResult Repository::abandonSession(const AbandonWrite& write) { return {false, notImplemented()}; }
Result<std::optional<FocusSession>> Repository::session(const SessionId& id) const { return {false, {}, notImplemented()}; }
Result<QVector<FocusSession>> Repository::pendingCompletions() const { return {false, {}, notImplemented()}; }
OperationResult Repository::recoverInterruptedSessions(const QDateTime& recoveredAtUtc) { return {false, notImplemented()}; }
Result<StatisticsSnapshot> Repository::statistics(const QDate& anchorDate) const { return {false, {}, notImplemented()}; }
Result<PetSnapshot> Repository::petSnapshot() const { return {false, {}, notImplemented()}; }
Result<SettlementReceipt> Repository::creditRewardAtomically(const RewardCreditRequest& request) { return {false, {}, notImplemented()}; }
Result<FeedReceipt> Repository::feedPetAtomically(const FeedCommand& command) { return {false, {}, notImplemented()}; }
}
