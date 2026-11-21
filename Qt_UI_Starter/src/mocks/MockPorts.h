#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
inline AppError notImplemented() { return {AppErrorCode::NotImplemented, QStringLiteral("演示模式：此业务由对应成员接入"), {}}; }
class MockRepository : public IRepository {
public:
 using IRepository::IRepository;
OperationResult open(const QString& databasePath) override { return {false, notImplemented()}; }
OperationResult close() override { return {false, notImplemented()}; }
Result<QVector<Task>> listTasks(const TaskFilter& filter) const override { return {true, {}, {}}; }
Result<std::optional<Task>> task(TaskId id) const override { return {true, {}, {}}; }
Result<Task> createTask(const TaskDraft& draft) override { return {false, {}, notImplemented()}; }
Result<Task> updateTask(TaskId id, const TaskDraft& draft) override { return {false, {}, notImplemented()}; }
OperationResult setTaskDone(TaskId id, bool completed) override { return {false, notImplemented()}; }
OperationResult softDeleteTask(TaskId id) override { return {false, notImplemented()}; }
Result<QVector<Task>> todayTasks(const QDate& date, int limit) const override { return {true, {}, {}}; }
Result<TaskCounts> taskCounts() const override { return {true, {}, {}}; }
OperationResult insertRunningSession(const FocusSession& session) override { return {false, notImplemented()}; }
OperationResult updateSessionActivity(const SessionId& id, SessionStatus status, int elapsedSeconds, const QDateTime& updatedAtUtc) override { return {false, notImplemented()}; }
OperationResult completeSession(const CompletionWrite& write) override { return {false, notImplemented()}; }
OperationResult abandonSession(const AbandonWrite& write) override { return {false, notImplemented()}; }
Result<std::optional<FocusSession>> session(const SessionId& id) const override { return {true, {}, {}}; }
Result<QVector<FocusSession>> pendingCompletions() const override { return {true, {}, {}}; }
OperationResult recoverInterruptedSessions(const QDateTime& recoveredAtUtc) override { return {false, notImplemented()}; }
Result<StatisticsSnapshot> statistics(const QDate& anchorDate) const override { StatisticsSnapshot s; for(int i=6;i>=0;--i) s.last7Days.push_back({QDate::currentDate().addDays(-i),0}); return {true,s,{}}; }
Result<PetSnapshot> petSnapshot() const override { return {true, {}, {}}; }
Result<SettlementReceipt> creditRewardAtomically(const RewardCreditRequest& request) override { return {false, {}, notImplemented()}; }
Result<FeedReceipt> feedPetAtomically(const FeedCommand& command) override { return {false, {}, notImplemented()}; }
};
class MockTaskManager : public ITaskService {
public:
 using ITaskService::ITaskService;
Result<QVector<Task>> listTasks(const TaskFilter& filter) const override { return {true, {}, {}}; }
Result<std::optional<Task>> task(TaskId id) const override { return {true, {}, {}}; }
Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id) override { return {false, {}, notImplemented()}; }
OperationResult setTaskDone(TaskId id, bool completed) override { return {false, notImplemented()}; }
OperationResult softDeleteTask(TaskId id) override { return {false, notImplemented()}; }
};
class MockStatisticsService : public IStatisticsService {
public:
 using IStatisticsService::IStatisticsService;
Result<StatisticsSnapshot> statistics() const override { StatisticsSnapshot s; for(int i=6;i>=0;--i) s.last7Days.push_back({QDate::currentDate().addDays(-i),0}); return {true,s,{}}; }
Result<QVector<Task>> todayTasks() const override { return {true, {}, {}}; }
};
class MockSettingsService : public ISettingsService {
public:
 using ISettingsService::ISettingsService;
OperationResult load() override { return {false, notImplemented()}; }
Preferences preferences() const override { return {}; }
OperationResult savePreferences(const Preferences& preferences) override { return {false, notImplemented()}; }
MusicPreferences musicPreferences() const override { return {}; }
OperationResult saveMusicPreferences(const MusicPreferences& preferences) override { return {false, notImplemented()}; }
};
class MockFocusEngine : public IFocusEngine {
public:
 using IFocusEngine::IFocusEngine;
FocusSnapshot snapshot() const override { return snapshot_; }
std::optional<CompletionResult> lastCompletion() const override { return {}; }
OperationResult configure(const FocusConfig& config) override { if(snapshot_.state!=FocusState::Idle && snapshot_.state!=FocusState::Completed) return {false,{AppErrorCode::ActiveSessionConflict,QStringLiteral("已有一轮专注，请先完成或放弃"),{}}}; if((config.mode==FocusMode::Focus && config.minutes!=25 && config.minutes!=50)||(config.mode==FocusMode::Custom && (config.minutes<15||config.minutes>90||config.minutes%5))||(config.mode==FocusMode::Rest && config.minutes!=5 && config.minutes!=10)) return {false,{AppErrorCode::Validation,QStringLiteral("请选择有效时长"),{}}}; snapshot_.config=config; snapshot_.plannedSeconds=snapshot_.remainingSeconds=config.minutes*60; emit focusChanged(snapshot_); return {true,{}}; }
OperationResult prepareNextRound(FocusMode mode) override { return {false, notImplemented()}; }
Result<SessionId> start() override { return {false, {}, notImplemented()}; }
OperationResult pause(const SessionId& expectedSessionId) override { return {false, notImplemented()}; }
OperationResult resume(const SessionId& expectedSessionId) override { return {false, notImplemented()}; }
OperationResult abandon(const SessionId& expectedSessionId) override { return {false, notImplemented()}; }
OperationResult retryCompletionSave(const SessionId& expectedSessionId) override { return {false, notImplemented()}; }
void applySettlementSuccess(const SettlementReceipt& receipt) override {  }
void applySettlementFailure(const SessionId& id, const AppError& error) override {  }
void onPreferencesChanged(fg::Preferences preferences) override {  }
private: FocusSnapshot snapshot_;
};
class MockRewardService : public IRewardService {
public:
 using IRewardService::IRewardService;
Result<SettlementReceipt> settleCompletedSession(const SessionId& sessionId) override { return {false, {}, notImplemented()}; }
};
class MockPetManager : public IPetService {
public:
 using IPetService::IPetService;
Result<PetSnapshot> pet() const override { return {true, {}, {}}; }
Result<FeedReceipt> feed() override { return {false, {}, notImplemented()}; }
};
class MockMusicController : public IMusicController {
public:
 using IMusicController::IMusicController;
OperationResult loadPlaylist() override { return {false, notImplemented()}; }
MusicSnapshot snapshot() const override { return {}; }
OperationResult importFiles(const QStringList& absolutePaths) override { return {false, notImplemented()}; }
OperationResult select(int index) override { return {false, notImplemented()}; }
OperationResult playPause() override { return {false, notImplemented()}; }
OperationResult stop() override { return {false, notImplemented()}; }
OperationResult previous() override { return {false, notImplemented()}; }
OperationResult next() override { return {false, notImplemented()}; }
OperationResult seek(qint64 positionMs) override { return {false, notImplemented()}; }
OperationResult setVolume(int volume) override { return {false, notImplemented()}; }
OperationResult remove(int index) override { return {false, notImplemented()}; }
};
class MockCompletionSound : public ICompletionSound {
public:
 using ICompletionSound::ICompletionSound;
void playForCompletedSession(const SessionId& sessionId) override {  }
};
}
