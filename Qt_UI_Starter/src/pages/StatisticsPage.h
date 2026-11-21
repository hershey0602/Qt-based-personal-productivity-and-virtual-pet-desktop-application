#pragma once
#include "ui/Components.h"
namespace fg { class StatisticsPage : public LayoutPage {
Q_OBJECT
public:
explicit StatisticsPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
public slots:
void revealHistory();
}; }
