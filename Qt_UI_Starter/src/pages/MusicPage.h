#pragma once
#include "ui/Components.h"
namespace fg { class MusicPage : public LayoutPage {
Q_OBJECT
public:
explicit MusicPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
public slots:
void renderMusic(fg::MusicSnapshot snapshot);
}; }
