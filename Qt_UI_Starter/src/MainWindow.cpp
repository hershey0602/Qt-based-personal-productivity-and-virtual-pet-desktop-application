#include "MainWindow.h"
#include "pages/DashboardPage.h"
#include "pages/TaskPage.h"
#include "pages/FocusPage.h"
#include "pages/PetPage.h"
#include "pages/StatisticsPage.h"
#include "pages/MusicPage.h"
#include "pages/SettingsPage.h"
#include "ui/TaskEditDialog.h"
#include "ui/ResultDialog.h"
namespace fg {
MainWindow::MainWindow(AppServices& s,QWidget* p):QMainWindow(p),services_(s){setWindowTitle(QStringLiteral("拾光岛 · Focus Garden"));setWindowIcon(QIcon(":/fg/UI_ASSETS/app_logo.svg"));resize(1366,820);setMinimumSize(1280,720);auto outer=new QFrame;outer->setProperty("role","outer");auto ol=new QHBoxLayout(outer);ol->setContentsMargins(12,12,12,12);ol->setSpacing(0);sidebar_=new Sidebar;ol->addWidget(sidebar_);auto right=new QWidget;auto l=new QVBoxLayout(right);l->setContentsMargins(0,0,0,0);l->setSpacing(0);l->addWidget(new TopHeader);notice_=new QLabel;notice_->setProperty("role","toast");notice_->setContentsMargins(12,0,12,0);notice_->setFixedHeight(40);notice_->setText(s.modelMode()==ModelMode::Demo?QStringLiteral("演示模式  ·  第 1 天公共框架"):QStringLiteral("本地数据已加载"));l->addWidget(notice_);stack_=new QStackedWidget;stack_->setObjectName("main_pages");l->addWidget(stack_);ol->addWidget(right,1);setCentralWidget(outer);
 auto dash=new DashboardPage(s);auto task=new TaskPage(s);auto focus=new FocusPage(s);auto pet=new PetPage(s);auto stats=new StatisticsPage(s);auto music=new MusicPage(s);auto settings=new SettingsPage(s);pages_={dash,task,focus,pet,stats,music,settings};for(auto page:pages_){auto scroll=new QScrollArea;scroll->setWidgetResizable(true);scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);scroll->setWidget(page);stack_->addWidget(scroll);}
 connect(sidebar_,&Sidebar::pageRequested,this,&MainWindow::navigate);connect(dash,&DashboardPage::navigationRequested,this,&MainWindow::navigate);connect(dash,&DashboardPage::newTaskRequested,this,&MainWindow::openNewTaskDialog);connect(dash,&DashboardPage::focusPresetRequested,this,&MainWindow::openFocusPreset);connect(dash,&DashboardPage::statisticsRequested,this,&MainWindow::showStatisticsHistory);connect(task,&TaskPage::newTaskRequested,this,&MainWindow::openNewTaskDialog);connect(task,&TaskPage::editTaskRequested,this,&MainWindow::openEditTaskDialog);connect(task,&TaskPage::deleteTaskRequested,this,&MainWindow::confirmDeleteTask);connect(task,&TaskPage::focusTaskRequested,this,&MainWindow::openFocusForTask);connect(focus,&FocusPage::statisticsRequested,this,&MainWindow::showStatisticsHistory);
 connect(&s,&AppServices::dataChanged,this,&MainWindow::refreshAll,Qt::QueuedConnection);connect(&s,&AppServices::focusChanged,focus,&FocusPage::refreshTimerOnly);connect(&s,&AppServices::musicChanged,music,&MusicPage::renderMusic);connect(&s,&AppServices::preferencesChanged,settings,&SettingsPage::renderPreferences);connect(&s,&AppServices::notice,this,&MainWindow::showNotice);connect(&s,&AppServices::focusCompleted,this,&MainWindow::onFocusCompleted);connect(&s,&AppServices::petLeveled,this,&MainWindow::onPetLeveled);navigate(PageId::Dashboard);
}
void MainWindow::navigate(fg::PageId id){if(int(id)<0||int(id)>=pages_.size())return;stack_->setCurrentIndex(int(id));sidebar_->select(id);}
void MainWindow::refreshAll(){for(auto page:pages_)QMetaObject::invokeMethod(page,"refresh",Qt::DirectConnection);}
void MainWindow::openNewTaskDialog(){TaskEditDialog d(services_,std::nullopt,this);connect(&d,&TaskEditDialog::taskSaved,this,&MainWindow::selectSavedTask);d.exec();}
void MainWindow::openEditTaskDialog(fg::TaskId id){TaskEditDialog d(services_,id,this);connect(&d,&TaskEditDialog::taskSaved,this,&MainWindow::selectSavedTask);d.exec();}
void MainWindow::confirmDeleteTask(fg::TaskId id){ConfirmDialog d(QStringLiteral("删除任务"),QStringLiteral("确定删除该任务吗？历史专注记录仍会保留。"),QStringLiteral("删除"),this);d.setCommitAction([this,id]{return services_.softDeleteTask(id);});d.exec();}
void MainWindow::openFocusForTask(fg::TaskId id){navigate(PageId::Focus);auto r=services_.preselectFocusTask(id);if(!r.ok)showNotice(r.error.message,NoticeLevel::Error);}
void MainWindow::openFocusPreset(int minutes){navigate(PageId::Focus);auto c=services_.focus().config;c.minutes=minutes;c.mode=FocusMode::Focus;c.durationSource=DurationSource::ExplicitSelection;auto r=services_.configureFocus(c);if(!r.ok)showNotice(r.error.message,NoticeLevel::Error);}
void MainWindow::showStatisticsHistory(){navigate(PageId::Statistics);static_cast<StatisticsPage*>(pages_[4])->revealHistory();}
void MainWindow::selectSavedTask(fg::TaskId id){static_cast<TaskPage*>(pages_[1])->selectTask(id);}
void MainWindow::onFocusCompleted(fg::CompletionResult r){if(stack_->currentIndex()==2){ResultDialog d(r,this);d.exec();}else showNotice(QStringLiteral("专注完成，奖励已到账"),NoticeLevel::Success);}
void MainWindow::onPetLeveled(fg::FeedReceipt r){ResultDialog d(r,this);d.exec();}
void MainWindow::showNotice(QString m,fg::NoticeLevel level){notice_->setText(m);notice_->setProperty("role",level==NoticeLevel::Error?"toastError":"toast");notice_->style()->unpolish(notice_);notice_->style()->polish(notice_);}
void MainWindow::closeEvent(QCloseEvent* e){auto s=services_.focus();if(s.state==FocusState::Running||s.state==FocusState::Paused){ConfirmDialog d(QStringLiteral("退出拾光岛"),QStringLiteral("当前专注尚未完成。退出将放弃本次专注，不获得金币。是否退出？"),QStringLiteral("退出"),this);const auto id=s.sessionId;d.setCommitAction([this,id]{return id?services_.abandonFocus(*id):OperationResult{false,{AppErrorCode::InvalidState,QStringLiteral("会话状态已改变，请重试"),{}}};});if(d.exec()!=QDialog::Accepted){e->ignore();return;}}else if(s.state==FocusState::SaveFailed&&s.sessionId){auto r=services_.retryFocusCompletion(*s.sessionId);if(!r.ok){showNotice(r.error.message,NoticeLevel::Error);e->ignore();return;}}e->accept();}
}
