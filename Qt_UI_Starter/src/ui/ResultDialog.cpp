#include "ResultDialog.h"
namespace fg {
void ResultDialog::content(const QString& title,const QString& text){setWindowTitle(title);setProperty("role","dialog");setMinimumWidth(400);auto l=new QVBoxLayout(this);l->setContentsMargins(28,28,28,28);auto s=new StateView;s->setState(":/fg/UI_ASSETS/states/state_success.svg",text);l->addWidget(s);auto b=createButton("result.close",QStringLiteral("继续加油"),ButtonRole::Primary);l->addWidget(b);connect(b,&QPushButton::clicked,this,&QDialog::accept);}
ResultDialog::ResultDialog(const fg::CompletionResult& r,QWidget* p):QDialog(p){content(QStringLiteral("专注完成"),r.rewardState==RewardState::Settled?QStringLiteral("完成 %1 分钟，获得 %2 金币").arg(r.minutes).arg(r.coins):QStringLiteral("专注已记录，金币结算待重试"));}
ResultDialog::ResultDialog(const fg::FeedReceipt& r,QWidget* p):QDialog(p){content(QStringLiteral("布丁长大啦"),QStringLiteral("布丁成长到了 Lv.%1").arg(r.levelAfter));}
}
