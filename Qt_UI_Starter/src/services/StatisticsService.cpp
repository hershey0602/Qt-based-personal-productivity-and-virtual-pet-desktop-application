#include "StatisticsService.h"

namespace fg {
StatisticsService::StatisticsService(IRepository& repository, QObject* parent)
    : IStatisticsService(parent), repository_(repository)
{
}

Result<StatisticsSnapshot> StatisticsService::statistics() const
{
    return repository_.statistics(QDate::currentDate());
}

Result<QVector<Task>> StatisticsService::todayTasks() const
{
    return repository_.todayTasks(QDate::currentDate(), 3);
}
}
