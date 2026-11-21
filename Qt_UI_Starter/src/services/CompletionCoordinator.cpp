#include "CompletionCoordinator.h"
namespace fg {
CompletionCoordinator::CompletionCoordinator(IFocusEngine& f,IRewardService& r,IRepository& db,QObject* p)
 : QObject(p),focus_(f),rewards_(r),repository_(db) {
 connect(&f,&IFocusEngine::completionPersisted,this,&CompletionCoordinator::settle);
}
void CompletionCoordinator::settle(fg::SessionId id) {
 auto r=rewards_.settleCompletedSession(id);
 if(r.ok) { focus_.applySettlementSuccess(r.value); emit settlementCompleted(r.value); }
 else { focus_.applySettlementFailure(id,r.error); emit settlementFailed(id,r.error); }
}
OperationResult CompletionCoordinator::retry(const SessionId& id) {
 const auto s=focus_.snapshot();
 if(s.state==FocusState::SaveFailed && s.sessionId==id) return focus_.retryCompletionSave(id);
 auto record=repository_.session(id);
 if(!record.ok) return {false,record.error};
 if(!record.value || record.value->status!=SessionStatus::Completed)
  return {false,{AppErrorCode::InvalidState,QStringLiteral("该专注尚未保存为完成记录"),{}}};
 auto r=rewards_.settleCompletedSession(id);
 if(r.ok){focus_.applySettlementSuccess(r.value);emit settlementCompleted(r.value);}
 else {focus_.applySettlementFailure(id,r.error);emit settlementFailed(id,r.error);}
 return {r.ok,r.error};
}
OperationResult CompletionCoordinator::retryPending() {
 auto rows=repository_.pendingCompletions(); if(!rows.ok)return {false,rows.error};
 for(const auto& row:rows.value){auto r=retry(row.sessionId);if(!r.ok)return r;}
 return {true,{}};
}
}
