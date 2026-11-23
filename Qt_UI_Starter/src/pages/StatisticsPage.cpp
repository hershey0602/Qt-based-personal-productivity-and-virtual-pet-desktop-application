#include "StatisticsPage.h"

namespace fg {
namespace {
QString sessionStatusText(SessionStatus status)
{
    return status == SessionStatus::Completed ? QStringLiteral("已完成")
         : status == SessionStatus::Abandoned ? QStringLiteral("已放弃")
                                               : QStringLiteral("进行中");
}
}

StatisticsPage::StatisticsPage(AppServices& services, QWidget* parent)
    : LayoutPage(services, "statistics", parent)
{
    refresh();
}

void StatisticsPage::refresh()
{
    refreshView();
    auto table = qobject_cast<QTableWidget*>(component("statistics.table"));
    if (!table)
        return;

    const auto result = services_.statistics();
    if (!result.ok) {
        showResult({false, result.error});
        table->setRowCount(0);
        return;
    }

    const auto history = result.value.history;
    table->setRowCount(history.size());
    for (int row = 0; row < history.size(); ++row) {
        const auto& item = history[row];
        const auto& session = item.session;
        const auto date = session.endedAtUtc
                              ? session.endedAtUtc->toLocalTime().toString(QStringLiteral("yyyy-MM-dd HH:mm"))
                              : QStringLiteral("进行中");
        auto task = session.taskTitleSnapshot.isEmpty() ? QStringLiteral("自由专注")
                                                         : session.taskTitleSnapshot;
        table->setItem(row, 0, new QTableWidgetItem(date));
        table->setItem(row, 1, new QTableWidgetItem(task));
        table->setItem(row, 2, new QTableWidgetItem(
                                   QStringLiteral("%1 分钟").arg(session.elapsedSeconds / 60)));
        table->setItem(row, 3, new QTableWidgetItem(sessionStatusText(session.status)));
        table->setItem(row, 4, new QTableWidgetItem(QStringLiteral("%1").arg(item.coins)));
    }
    table->resizeRowsToContents();
}

void StatisticsPage::revealHistory()
{
    if (auto area = qobject_cast<QScrollArea*>(parentWidget()->parentWidget()))
        area->ensureWidgetVisible(component("statistics.table"));
}
}
