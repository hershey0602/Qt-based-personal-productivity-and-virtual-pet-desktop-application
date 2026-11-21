#include "FocusPage.h"
namespace fg {
FocusPage::FocusPage(AppServices& services,QWidget* parent):LayoutPage(services,"focus",parent){
connect(this,&LayoutPage::actionRequested,this,[this](QString a){
 if(a=="mode.25"||a=="mode.50"||a=="mode.rest"){FocusConfig c;c.minutes=a=="mode.rest"?services_.preferences().defaultRestMinutes:a.mid(5).toInt();c.mode=a=="mode.rest"?FocusMode::Rest:FocusMode::Focus;c.durationSource=DurationSource::ExplicitSelection;showResult(services_.configureFocus(c));}
 else if(a=="mode.custom"){auto c=component("focus.customMinutes");c->setVisible(true);}
 else if(a=="focus.start"){auto r=services_.startFocus();showResult({r.ok,r.error});}
 else if(a=="nav.statistics")emit statisticsRequested();
 else if(a.startsWith("focus."))showResult({false,{AppErrorCode::NotImplemented,QStringLiteral("演示模式：等待专注业务接入"),{}}});
 });
 connect(qobject_cast<QComboBox*>(component("focus.customMinutes")),&QComboBox::currentIndexChanged,this,[this](int){FocusConfig c;c.mode=FocusMode::Custom;c.minutes=qobject_cast<QComboBox*>(component("focus.customMinutes"))->currentText().toInt();c.durationSource=DurationSource::ExplicitSelection;showResult(services_.configureFocus(c));});
refresh();
}
void FocusPage::refresh(){refreshView();refreshTimerOnly(services_.focus());}
void FocusPage::refreshTimerOnly(fg::FocusSnapshot s){for(auto ring:findChildren<TimerRing*>())ring->setSnapshot(s);
if(auto reward=qobject_cast<QLabel*>(component("focus.rewardValue")))reward->setText(QStringLiteral("%1 金币").arg(s.config.mode==FocusMode::Rest?0:s.config.minutes/5*4));
const QStringList ids={"focus.start","focus.pause","focus.resume","focus.abandon"};
for(int i=0;i<ids.size();++i)if(auto w=component(ids[i]))w->setEnabled(i==0?s.state==FocusState::Idle:i==1?s.state==FocusState::Running:i==2?s.state==FocusState::Paused:(s.state==FocusState::Running||s.state==FocusState::Paused));
if(auto label=qobject_cast<QLabel*>(component("focus.state")))label->setText(s.state==FocusState::Idle?QStringLiteral("待开始"):s.state==FocusState::Running?QStringLiteral("专注中"):s.state==FocusState::Paused?QStringLiteral("已暂停"):QStringLiteral("等待处理"));
}
}
