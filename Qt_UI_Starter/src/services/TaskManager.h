#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class TaskManager : public ITaskService {
 Q_OBJECT
public:
 explicit TaskManager(IRepository& repository, QObject* parent=nullptr);
Result<QVector<Task>> listTasks(const TaskFilter& filter) const override;
Result<std::optional<Task>> task(TaskId id) const override;
Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id) override;
OperationResult setTaskDone(TaskId id, bool completed) override;
OperationResult softDeleteTask(TaskId id) override;
private:
 IRepository& repository_;
};
}
