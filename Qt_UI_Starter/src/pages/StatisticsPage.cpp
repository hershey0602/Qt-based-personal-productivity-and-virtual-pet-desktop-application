#include "StatisticsPage.h"
namespace fg {
StatisticsPage::StatisticsPage(AppServices& services,QWidget* parent):LayoutPage(services,"statistics",parent){

refresh();
}
void StatisticsPage::refresh(){refreshView();}
void StatisticsPage::revealHistory(){if(auto area=qobject_cast<QScrollArea*>(parentWidget()->parentWidget()))area->ensureWidgetVisible(component("statistics.table"));}
}
