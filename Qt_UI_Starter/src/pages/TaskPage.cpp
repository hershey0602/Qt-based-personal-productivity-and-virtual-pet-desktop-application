#include "TaskPage.h"
namespace fg {
TaskPage::TaskPage(AppServices& services,QWidget* parent):LayoutPage(services,"tasks",parent){
connect(this,&LayoutPage::actionRequested,this,[this](QString a){if(a=="task.add")emit newTaskRequested();});
refresh();
}
void TaskPage::refresh(){refreshView();}
void TaskPage::selectTask(fg::TaskId id){auto t=services_.task(id);setProperty("selectedTaskId",t.ok&&t.value?QVariant(id):QVariant());}
}
