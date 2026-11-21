#pragma once
#include "ui/Components.h"
namespace fg { class PetPage : public LayoutPage {
Q_OBJECT
public:
explicit PetPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();

}; }
