#include "TaskManager.h"

namespace fg {
namespace {
AppError validationError(const QString& message)
{
    return {AppErrorCode::Validation, message, {}};
}

AppError missingError()
{
    return {AppErrorCode::NotFound, QStringLiteral("任务不存在或已删除"), {}};
}
}

TaskManager::TaskManager(IRepository& repository, QObject* parent)
    : ITaskService(parent), repository_(repository)
{
}

Result<QVector<Task>> TaskManager::listTasks(const TaskFilter& filter) const
{
    return repository_.listTasks(filter);
}

Result<std::optional<Task>> TaskManager::task(TaskId id) const
{
    if (id <= 0) return {false, {}, validationError(QStringLiteral("任务编号无效"))};
    auto result = repository_.task(id);
    if (result.ok && result.value && result.value->deleted)
        return {true, std::nullopt, {}};
    return result;
}

Result<Task> TaskManager::saveTask(const TaskDraft& draft, std::optional<TaskId> id)
{
    const int titleLength = draft.title.trimmed().toUcs4().size();
    if (titleLength < 1 || titleLength > 100)
        return {false, {}, validationError(QStringLiteral("任务标题需为1到100个字符"))};
    if (draft.note.toUcs4().size() > 500)
        return {false, {}, validationError(QStringLiteral("任务备注不能超过500个字符"))};

    Result<Task> result;
    if (id) {
        if (*id <= 0) return {false, {}, validationError(QStringLiteral("任务编号无效"))};
        auto current = repository_.task(*id);
        if (!current.ok) return {false, {}, current.error};
        if (!current.value || current.value->deleted)
            return {false, {}, missingError()};
        result = repository_.updateTask(*id, draft);
    } else {
        result = repository_.createTask(draft);
    }
    if (result.ok) emit tasksChanged();
    else emit errorOccurred(result.error);
    return result;
}

OperationResult TaskManager::setTaskDone(TaskId id, bool completed)
{
    if (id <= 0) return {false, validationError(QStringLiteral("任务编号无效"))};
    auto result = repository_.setTaskDone(id, completed);
    if (result.ok) emit tasksChanged();
    else emit errorOccurred(result.error);
    return result;
}

OperationResult TaskManager::softDeleteTask(TaskId id)
{
    if (id <= 0) return {false, validationError(QStringLiteral("任务编号无效"))};
    auto result = repository_.softDeleteTask(id);
    if (result.ok) emit tasksChanged();
    else emit errorOccurred(result.error);
    return result;
}
}
