#include "TaskManager.h"
#include "mocks/MockPorts.h"
namespace fg {
TaskManager::TaskManager(IRepository& repository, QObject* parent) : ITaskService(parent) {}
Result<QVector<Task>> TaskManager::listTasks(const TaskFilter& filter) const { return {false, {}, notImplemented()}; }
Result<std::optional<Task>> TaskManager::task(TaskId id) const { return {false, {}, notImplemented()}; }
Result<Task> TaskManager::saveTask(const TaskDraft& draft, std::optional<TaskId> id) { return {false, {}, notImplemented()}; }
OperationResult TaskManager::setTaskDone(TaskId id, bool completed) { return {false, notImplemented()}; }
OperationResult TaskManager::softDeleteTask(TaskId id) { return {false, notImplemented()}; }
}
