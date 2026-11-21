#include "StatisticsService.h"
#include "mocks/MockPorts.h"
namespace fg {
StatisticsService::StatisticsService(IRepository& repository, QObject* parent) : IStatisticsService(parent) {}
Result<StatisticsSnapshot> StatisticsService::statistics() const { return {false, {}, notImplemented()}; }
Result<QVector<Task>> StatisticsService::todayTasks() const { return {false, {}, notImplemented()}; }
}
