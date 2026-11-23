#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class StatisticsService : public IStatisticsService {
 Q_OBJECT
public:
 explicit StatisticsService(IRepository& repository, QObject* parent=nullptr);
Result<StatisticsSnapshot> statistics() const override;
Result<QVector<Task>> todayTasks() const override;
private:
 IRepository& repository_;
};
}
