#pragma once
#include "contracts/ServicePorts.h"
#include "CompletionCoordinator.h"
namespace fg { class AppServices : public QObject {
 Q_OBJECT
public:
 AppServices(ITaskService& tasks, IStatisticsService& statistics, IFocusEngine& focus, IPetService& pet, ISettingsService& settings, IMusicController& music, CompletionCoordinator& completion, ModelMode mode, QObject* parent=nullptr);
Result<QVector<Task>> listTasks(const TaskFilter& filter) const;
Result<std::optional<Task>> task(TaskId id) const;
Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id);
OperationResult setTaskDone(TaskId id, bool completed);
OperationResult softDeleteTask(TaskId id);
Result<QVector<Task>> todayTasks() const;
Result<StatisticsSnapshot> statistics() const;
FocusSnapshot focus() const;
std::optional<CompletionResult> lastCompletion() const;
OperationResult configureFocus(const FocusConfig& config);
OperationResult preselectFocusTask(TaskId id);
OperationResult prepareNextRound(FocusMode mode);
Result<SessionId> startFocus();
OperationResult pauseFocus(const SessionId& expectedSessionId);
OperationResult resumeFocus(const SessionId& expectedSessionId);
OperationResult abandonFocus(const SessionId& expectedSessionId);
OperationResult retryFocusCompletion(const SessionId& sessionId);
Result<PetSnapshot> pet() const;
Result<FeedReceipt> feedPet();
Preferences preferences() const;
OperationResult savePreferences(const Preferences& preferences);
IMusicController& music() const;
ModelMode modelMode() const;
signals:
void dataChanged();
void focusChanged(fg::FocusSnapshot snapshot);
void preferencesChanged(fg::Preferences preferences);
void musicChanged(fg::MusicSnapshot snapshot);
void focusCompleted(fg::CompletionResult result);
void petLeveled(fg::FeedReceipt receipt);
void notice(QString message, fg::NoticeLevel level);
private slots:
void onTasksChanged();
void onPreferencesChanged(fg::Preferences preferences);
void onFocusChanged(fg::FocusSnapshot snapshot);
void onSettlementCompleted(fg::SettlementReceipt receipt);
void onSettlementFailed(fg::SessionId sessionId, fg::AppError error);
void onPetChanged(fg::PetSnapshot pet);
void onPetLeveled(fg::FeedReceipt receipt);
void onMusicChanged(fg::MusicSnapshot snapshot);
void onRestFinished();
void onServiceError(fg::AppError error);
private:
ITaskService& tasks_; IStatisticsService& statistics_; IFocusEngine& focus_; IPetService& pet_; ISettingsService& settings_; IMusicController& music_; CompletionCoordinator& completion_; ModelMode mode_;
}; }
