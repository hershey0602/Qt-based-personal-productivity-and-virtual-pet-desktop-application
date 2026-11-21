#include "DashboardPage.h"
namespace fg {
DashboardPage::DashboardPage(AppServices& services,QWidget* parent):LayoutPage(services,"dashboard",parent){
connect(this,&LayoutPage::actionRequested,this,[this](QString a){
 if(a=="task.add")emit newTaskRequested();
 else if(a=="quick.25"||a=="quick.50")emit focusPresetRequested(a.mid(6).toInt());
 else if(a=="nav.statistics")emit statisticsRequested();
 else if(a=="nav.tasks")emit navigationRequested(PageId::Tasks);
 else if(a=="nav.pet")emit navigationRequested(PageId::Pet);
 else if(a=="nav.music")emit navigationRequested(PageId::Music);
});
refresh();
}
void DashboardPage::refresh(){refreshView();}
}
