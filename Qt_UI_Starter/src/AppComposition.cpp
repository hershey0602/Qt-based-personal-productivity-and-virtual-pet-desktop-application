#include "AppComposition.h"
#include "mocks/MockPorts.h"
namespace fg {
AppComposition::AppComposition(const QString& d,fg::ModelMode mode,QObject* p):QObject(p),directory_(d),mode_(mode){}
OperationResult AppComposition::initialize(){if(services_)return {true,{}};
 // B/C/D接管后在此替换Mock。Live需依次load/open/recover/loadPlaylist/retryPending，失败终止启动。
 if(mode_==ModelMode::Live)return {false,{AppErrorCode::NotImplemented,QStringLiteral("当前为第1天公共框架，请以演示模式运行"),{}}};
 auto repo=new MockRepository(this);auto settings=new MockSettingsService(this);auto tasks=new MockTaskManager(this);auto statistics=new MockStatisticsService(this);auto focus=new MockFocusEngine(this);auto rewards=new MockRewardService(this);auto pet=new MockPetManager(this);auto music=new MockMusicController(this);auto sound=new MockCompletionSound(this);
 auto completion=new CompletionCoordinator(*focus,*rewards,*repo,this);
 services_=new AppServices(*tasks,*statistics,*focus,*pet,*settings,*music,*completion,mode_,this);
 connect(focus,&IFocusEngine::completionPersisted,sound,&ICompletionSound::playForCompletedSession);
 connect(rewards,&IRewardService::errorOccurred,services_,[this](AppError e){emit services_->notice(e.message,NoticeLevel::Error);});
 connect(sound,&ICompletionSound::errorOccurred,services_,[this](AppError e){emit services_->notice(e.message,NoticeLevel::Error);});
 return {true,{}};
}
AppServices& AppComposition::services()const{Q_ASSERT(services_);return *services_;}
void AppComposition::shutdown(){services_=nullptr;qDeleteAll(children());}
}
