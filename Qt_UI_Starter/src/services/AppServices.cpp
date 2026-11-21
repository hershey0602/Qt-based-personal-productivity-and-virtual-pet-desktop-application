#include "AppServices.h"
namespace fg {
AppServices::AppServices(ITaskService& t,IStatisticsService& st,IFocusEngine& f,IPetService& p,ISettingsService& s,IMusicController& m,CompletionCoordinator& c,ModelMode mode,QObject* parent)
 :QObject(parent),tasks_(t),statistics_(st),focus_(f),pet_(p),settings_(s),music_(m),completion_(c),mode_(mode){
 connect(&t,&ITaskService::tasksChanged,this,&AppServices::onTasksChanged);
 connect(&s,&ISettingsService::preferencesChanged,this,&AppServices::onPreferencesChanged);
 connect(&s,&ISettingsService::preferencesChanged,&f,&IFocusEngine::onPreferencesChanged);
 connect(&f,&IFocusEngine::focusChanged,this,&AppServices::onFocusChanged);
 connect(&f,&IFocusEngine::restFinished,this,&AppServices::onRestFinished);
 connect(&c,&CompletionCoordinator::settlementCompleted,this,&AppServices::onSettlementCompleted);
 connect(&c,&CompletionCoordinator::settlementFailed,this,&AppServices::onSettlementFailed);
 connect(&p,&IPetService::petChanged,this,&AppServices::onPetChanged);
 connect(&p,&IPetService::petLeveled,this,&AppServices::onPetLeveled);
 connect(&m,&IMusicController::musicChanged,this,&AppServices::onMusicChanged);
 connect(&t,&ITaskService::errorOccurred,this,&AppServices::onServiceError);
 connect(&s,&ISettingsService::errorOccurred,this,&AppServices::onServiceError);
 connect(&f,&IFocusEngine::errorOccurred,this,&AppServices::onServiceError);
 connect(&p,&IPetService::errorOccurred,this,&AppServices::onServiceError);
 connect(&m,&IMusicController::errorOccurred,this,&AppServices::onServiceError);
}
Result<QVector<Task>> AppServices::listTasks(const TaskFilter& f)const{return tasks_.listTasks(f);}
Result<std::optional<Task>> AppServices::task(TaskId id)const{return tasks_.task(id);}
Result<Task> AppServices::saveTask(const TaskDraft& d,std::optional<TaskId> id){return tasks_.saveTask(d,id);}
OperationResult AppServices::setTaskDone(TaskId id,bool done){return tasks_.setTaskDone(id,done);}
OperationResult AppServices::softDeleteTask(TaskId id){return tasks_.softDeleteTask(id);}
Result<QVector<Task>> AppServices::todayTasks()const{return statistics_.todayTasks();}
Result<StatisticsSnapshot> AppServices::statistics()const{return statistics_.statistics();}
FocusSnapshot AppServices::focus()const{return focus_.snapshot();}
std::optional<CompletionResult> AppServices::lastCompletion()const{return focus_.lastCompletion();}
OperationResult AppServices::configureFocus(const FocusConfig& c){return focus_.configure(c);}
OperationResult AppServices::preselectFocusTask(TaskId id){
 const auto s=focus();
 if(s.state!=FocusState::Idle && s.state!=FocusState::Completed)return {false,{AppErrorCode::ActiveSessionConflict,QStringLiteral("已有一轮专注，请先完成或放弃"),{}}};
 auto t=task(id);if(!t.ok)return {false,t.error};
 if(!t.value || t.value->completed || t.value->deleted)return {false,{AppErrorCode::NotFound,QStringLiteral("请选择未完成任务"),{}}};
 FocusConfig c;c.taskId=id;c.minutes=preferences().defaultFocusMinutes;return configureFocus(c);
}
OperationResult AppServices::prepareNextRound(FocusMode m){return focus_.prepareNextRound(m);}
Result<SessionId> AppServices::startFocus(){return focus_.start();}
OperationResult AppServices::pauseFocus(const SessionId& id){return focus_.pause(id);}
OperationResult AppServices::resumeFocus(const SessionId& id){return focus_.resume(id);}
OperationResult AppServices::abandonFocus(const SessionId& id){return focus_.abandon(id);}
OperationResult AppServices::retryFocusCompletion(const SessionId& id){return completion_.retry(id);}
Result<PetSnapshot> AppServices::pet()const{return pet_.pet();}
Result<FeedReceipt> AppServices::feedPet(){return pet_.feed();}
Preferences AppServices::preferences()const{return settings_.preferences();}
OperationResult AppServices::savePreferences(const Preferences& p){return settings_.savePreferences(p);}
IMusicController& AppServices::music()const{return music_;}
ModelMode AppServices::modelMode()const{return mode_;}
void AppServices::onTasksChanged(){emit dataChanged();}
void AppServices::onPreferencesChanged(fg::Preferences p){emit preferencesChanged(p);}
void AppServices::onFocusChanged(fg::FocusSnapshot s){emit focusChanged(s);}
void AppServices::onSettlementCompleted(fg::SettlementReceipt r){emit dataChanged();if(r.newlyCredited){auto s=focus_.lastCompletion(); if(s && s->sessionId==r.sessionId)emit focusCompleted(*s);else {emit notice(QStringLiteral("已补发专注奖励 %1 金币").arg(r.coins),NoticeLevel::Success);}}}
void AppServices::onSettlementFailed(fg::SessionId,fg::AppError e){emit notice(e.message,NoticeLevel::Error);}
void AppServices::onPetChanged(fg::PetSnapshot){emit dataChanged();}
void AppServices::onPetLeveled(fg::FeedReceipt r){emit petLeveled(r);}
void AppServices::onMusicChanged(fg::MusicSnapshot s){emit musicChanged(s);}
void AppServices::onRestFinished(){emit notice(QStringLiteral("休息结束，准备好再出发"),NoticeLevel::Info);}
void AppServices::onServiceError(fg::AppError e){qWarning().noquote()<<e.diagnostic;emit notice(e.message,NoticeLevel::Error);}
}
