#pragma once
#include "ui/Components.h"
#include <optional>
namespace fg { class TaskPage : public LayoutPage {
Q_OBJECT
public:
explicit TaskPage(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void refresh();
void selectTask(fg::TaskId taskId);
signals:
void newTaskRequested();
void editTaskRequested(fg::TaskId taskId);
void deleteTaskRequested(fg::TaskId taskId);
void focusTaskRequested(fg::TaskId taskId);
private:
TaskFilter filter_;
std::optional<TaskId> selectedTaskId_;
bool rendering_ = false;
void renderRows(const QVector<Task>& tasks);
void renderDetails();
}; }
