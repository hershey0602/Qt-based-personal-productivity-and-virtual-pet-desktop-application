#pragma once
#include "DomainTypes.h"
namespace fg {
class IRepository : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IRepository() = default;
 virtual OperationResult open(const QString& databasePath) = 0;
 virtual OperationResult close() = 0;
 virtual Result<QVector<Task>> listTasks(const TaskFilter& filter) const = 0;
 virtual Result<std::optional<Task>> task(TaskId id) const = 0;
 virtual Result<Task> createTask(const TaskDraft& draft) = 0;
 virtual Result<Task> updateTask(TaskId id, const TaskDraft& draft) = 0;
 virtual OperationResult setTaskDone(TaskId id, bool completed) = 0;
 virtual OperationResult softDeleteTask(TaskId id) = 0;
 virtual Result<QVector<Task>> todayTasks(const QDate& date, int limit) const = 0;
 virtual Result<TaskCounts> taskCounts() const = 0;
 virtual OperationResult insertRunningSession(const FocusSession& session) = 0;
 virtual OperationResult updateSessionActivity(const SessionId& id, SessionStatus status, int elapsedSeconds, const QDateTime& updatedAtUtc) = 0;
 virtual OperationResult completeSession(const CompletionWrite& write) = 0;
 virtual OperationResult abandonSession(const AbandonWrite& write) = 0;
 virtual Result<std::optional<FocusSession>> session(const SessionId& id) const = 0;
 virtual Result<QVector<FocusSession>> pendingCompletions() const = 0;
 virtual OperationResult recoverInterruptedSessions(const QDateTime& recoveredAtUtc) = 0;
 virtual Result<StatisticsSnapshot> statistics(const QDate& anchorDate) const = 0;
 virtual Result<PetSnapshot> petSnapshot() const = 0;
 virtual Result<SettlementReceipt> creditRewardAtomically(const RewardCreditRequest& request) = 0;
 virtual Result<FeedReceipt> feedPetAtomically(const FeedCommand& command) = 0;
};
class ITaskService : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~ITaskService() = default;
 virtual Result<QVector<Task>> listTasks(const TaskFilter& filter) const = 0;
 virtual Result<std::optional<Task>> task(TaskId id) const = 0;
 virtual Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id) = 0;
 virtual OperationResult setTaskDone(TaskId id, bool completed) = 0;
 virtual OperationResult softDeleteTask(TaskId id) = 0;
signals:
void tasksChanged();
void errorOccurred(fg::AppError error);
};
class IStatisticsService : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IStatisticsService() = default;
 virtual Result<StatisticsSnapshot> statistics() const = 0;
 virtual Result<QVector<Task>> todayTasks() const = 0;
};
class ISettingsService : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~ISettingsService() = default;
 virtual OperationResult load() = 0;
 virtual Preferences preferences() const = 0;
 virtual OperationResult savePreferences(const Preferences& preferences) = 0;
 virtual MusicPreferences musicPreferences() const = 0;
 virtual OperationResult saveMusicPreferences(const MusicPreferences& preferences) = 0;
signals:
void preferencesChanged(fg::Preferences preferences);
void musicPreferencesChanged(fg::MusicPreferences preferences);
void errorOccurred(fg::AppError error);
};
class IFocusEngine : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IFocusEngine() = default;
 virtual FocusSnapshot snapshot() const = 0;
 virtual std::optional<CompletionResult> lastCompletion() const = 0;
 virtual OperationResult configure(const FocusConfig& config) = 0;
 virtual OperationResult prepareNextRound(FocusMode mode) = 0;
 virtual Result<SessionId> start() = 0;
 virtual OperationResult pause(const SessionId& expectedSessionId) = 0;
 virtual OperationResult resume(const SessionId& expectedSessionId) = 0;
 virtual OperationResult abandon(const SessionId& expectedSessionId) = 0;
 virtual OperationResult retryCompletionSave(const SessionId& expectedSessionId) = 0;
 virtual void applySettlementSuccess(const SettlementReceipt& receipt) = 0;
 virtual void applySettlementFailure(const SessionId& id, const AppError& error) = 0;
public slots:
 virtual void onPreferencesChanged(fg::Preferences preferences) = 0;
signals:
void focusChanged(fg::FocusSnapshot snapshot);
void completionPersisted(fg::SessionId sessionId);
void restFinished();
void errorOccurred(fg::AppError error);
};
class IRewardService : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IRewardService() = default;
 virtual Result<SettlementReceipt> settleCompletedSession(const SessionId& sessionId) = 0;
signals:
void rewardCommitted(fg::SettlementReceipt receipt);
void errorOccurred(fg::AppError error);
};
class IPetService : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IPetService() = default;
 virtual Result<PetSnapshot> pet() const = 0;
 virtual Result<FeedReceipt> feed() = 0;
signals:
void petChanged(fg::PetSnapshot pet);
void petLeveled(fg::FeedReceipt receipt);
void errorOccurred(fg::AppError error);
};
class IMusicController : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~IMusicController() = default;
 virtual OperationResult loadPlaylist() = 0;
 virtual MusicSnapshot snapshot() const = 0;
 virtual OperationResult importFiles(const QStringList& absolutePaths) = 0;
 virtual OperationResult select(int index) = 0;
 virtual OperationResult playPause() = 0;
 virtual OperationResult stop() = 0;
 virtual OperationResult previous() = 0;
 virtual OperationResult next() = 0;
 virtual OperationResult seek(qint64 positionMs) = 0;
 virtual OperationResult setVolume(int volume) = 0;
 virtual OperationResult remove(int index) = 0;
signals:
void musicChanged(fg::MusicSnapshot snapshot);
void errorOccurred(fg::AppError error);
};
class ICompletionSound : public QObject {
 Q_OBJECT
public:
 using QObject::QObject;
 virtual ~ICompletionSound() = default;
 virtual void playForCompletedSession(const SessionId& sessionId) = 0;
signals:
void errorOccurred(fg::AppError error);
};
}
