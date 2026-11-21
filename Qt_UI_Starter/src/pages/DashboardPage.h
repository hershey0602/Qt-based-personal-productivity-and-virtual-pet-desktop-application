#pragma once
#include "ui/Components.h"
namespace fg { class DashboardPage : public LayoutPage {
Q_OBJECT
public:
explicit DashboardPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
signals:
void navigationRequested(fg::PageId page);
void focusPresetRequested(int minutes);
void newTaskRequested();
void statisticsRequested();
}; }
