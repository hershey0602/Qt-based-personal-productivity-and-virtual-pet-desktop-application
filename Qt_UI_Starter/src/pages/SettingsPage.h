#pragma once
#include "ui/Components.h"
namespace fg { class SettingsPage : public LayoutPage {
Q_OBJECT
public:
explicit SettingsPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
public slots:
void renderPreferences(fg::Preferences preferences);
}; }
