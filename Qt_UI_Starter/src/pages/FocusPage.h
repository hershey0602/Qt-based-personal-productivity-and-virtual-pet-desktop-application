#pragma once
#include "ui/Components.h"
namespace fg { class FocusPage : public LayoutPage {
Q_OBJECT
public:
explicit FocusPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
public slots:
void refreshTimerOnly(fg::FocusSnapshot snapshot);
signals:
void statisticsRequested();
}; }
