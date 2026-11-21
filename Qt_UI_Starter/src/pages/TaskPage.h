#pragma once
#include "ui/Components.h"
namespace fg { class TaskPage : public LayoutPage {
Q_OBJECT
public:
explicit TaskPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
public slots:
void selectTask(fg::TaskId taskId);
signals:
void newTaskRequested();
void editTaskRequested(fg::TaskId taskId);
void deleteTaskRequested(fg::TaskId taskId);
void focusTaskRequested(fg::TaskId taskId);
}; }
