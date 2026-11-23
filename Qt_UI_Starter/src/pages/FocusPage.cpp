#include "FocusPage.h"

namespace fg {
FocusPage::FocusPage(AppServices& services, QWidget* parent)
    : LayoutPage(services, "focus", parent)
{
    connect(this, &LayoutPage::actionRequested, this, [this](const QString& action) {
        if (action == QStringLiteral("mode.25") || action == QStringLiteral("mode.50")
            || action == QStringLiteral("mode.rest")) {
            FocusConfig config = services_.focus().config;
            config.minutes = action == QStringLiteral("mode.rest")
                                 ? services_.preferences().defaultRestMinutes
                                 : action.mid(5).toInt();
            config.mode = action == QStringLiteral("mode.rest") ? FocusMode::Rest : FocusMode::Focus;
            config.durationSource = DurationSource::ExplicitSelection;
            showResult(services_.configureFocus(config));
        } else if (action == QStringLiteral("mode.custom")) {
            if (auto custom = component("focus.customMinutes"))
                custom->setVisible(true);
        } else if (action == QStringLiteral("focus.start")) {
            const auto result = services_.startFocus();
            showResult({result.ok, result.error});
        } else if (action == QStringLiteral("focus.pause")) {
            const auto snapshot = services_.focus();
            if (snapshot.sessionId)
                showResult(services_.pauseFocus(*snapshot.sessionId));
        } else if (action == QStringLiteral("focus.resume")) {
            const auto snapshot = services_.focus();
            if (snapshot.sessionId)
                showResult(services_.resumeFocus(*snapshot.sessionId));
        } else if (action == QStringLiteral("focus.abandon")) {
            const auto snapshot = services_.focus();
            if (!snapshot.sessionId)
                return;
            auto dialog = new QDialog(this);
            dialog->setWindowTitle(QStringLiteral("放弃专注"));
            auto layout = new QVBoxLayout(dialog);
            layout->addWidget(new QLabel(QStringLiteral("确定放弃本轮专注吗？放弃后不会获得金币。"), dialog));
            auto buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, dialog);
            buttons->button(QDialogButtonBox::Ok)->setText(QStringLiteral("放弃"));
            buttons->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("取消"));
            layout->addWidget(buttons);
            connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
            connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
            if (dialog->exec() == QDialog::Accepted)
                showResult(services_.abandonFocus(*snapshot.sessionId));
            dialog->deleteLater();
        } else if (action == QStringLiteral("focus.retry")) {
            const auto snapshot = services_.focus();
            if (snapshot.sessionId)
                showResult(services_.retryFocusCompletion(*snapshot.sessionId));
        } else if (action == QStringLiteral("nav.statistics")) {
            emit statisticsRequested();
        }
    });

    connect(qobject_cast<QComboBox*>(component("focus.customMinutes")),
            &QComboBox::currentIndexChanged, this, [this](int) {
                auto combo = qobject_cast<QComboBox*>(component("focus.customMinutes"));
                FocusConfig config = services_.focus().config;
                config.mode = FocusMode::Custom;
                config.minutes = combo->currentText().toInt();
                config.durationSource = DurationSource::ExplicitSelection;
                showResult(services_.configureFocus(config));
            });
    connect(qobject_cast<QComboBox*>(component("focus.task")),
            &QComboBox::currentIndexChanged, this, [this](int index) {
                auto combo = qobject_cast<QComboBox*>(component("focus.task"));
                if (index < 0)
                    return;
                if (index == 0) {
                    auto config = services_.focus().config;
                    config.taskId.reset();
                    showResult(services_.configureFocus(config));
                    return;
                }
                const auto id = combo->itemData(index).toLongLong();
                showResult(services_.preselectFocusTask(id));
            });
    refresh();
}

void FocusPage::refresh()
{
    refreshView();
    auto taskCombo = qobject_cast<QComboBox*>(component("focus.task"));
    if (taskCombo) {
        const auto current = services_.focus().config.taskId;
        QSignalBlocker blocker(taskCombo);
        taskCombo->clear();
        taskCombo->addItem(QStringLiteral("不关联任务"));
        const auto result = services_.listTasks(TaskFilter{});
        if (result.ok) {
            for (const auto& task : result.value) {
                if (task.completed || task.deleted)
                    continue;
                taskCombo->addItem(task.title, task.id);
            }
        }
        if (current) {
            const auto index = taskCombo->findData(*current);
            taskCombo->setCurrentIndex(index >= 0 ? index : 0);
        }
    }
    refreshTimerOnly(services_.focus());
}

void FocusPage::refreshTimerOnly(fg::FocusSnapshot snapshot)
{
    for (auto ring : findChildren<TimerRing*>())
        ring->setSnapshot(snapshot);
    if (auto reward = qobject_cast<QLabel*>(component("focus.rewardValue")))
        reward->setText(QStringLiteral("%1 金币")
                            .arg(snapshot.config.mode == FocusMode::Rest
                                     ? 0
                                     : snapshot.config.minutes / 5 * 4));

    const QStringList ids = {QStringLiteral("focus.start"), QStringLiteral("focus.pause"),
                             QStringLiteral("focus.resume"), QStringLiteral("focus.abandon")};
    for (int i = 0; i < ids.size(); ++i) {
        if (auto widget = component(ids[i])) {
            const bool enabled = i == 0 ? snapshot.state == FocusState::Idle
                              : i == 1 ? snapshot.state == FocusState::Running
                              : i == 2 ? snapshot.state == FocusState::Paused
                                        : snapshot.state == FocusState::Running
                                              || snapshot.state == FocusState::Paused;
            widget->setEnabled(enabled);
        }
    }
    if (auto retry = component("focus.retry"))
        retry->setEnabled(snapshot.state == FocusState::SaveFailed);
    if (auto errorRow = component("focus.errorRow"))
        errorRow->setVisible(snapshot.state == FocusState::SaveFailed);
    const bool planEditable = snapshot.state == FocusState::Idle
                              || snapshot.state == FocusState::Completed;
    for (const QString& id : {QStringLiteral("focus.mode25"), QStringLiteral("focus.mode50"),
                              QStringLiteral("focus.custom"), QStringLiteral("focus.rest"),
                              QStringLiteral("focus.customMinutes"), QStringLiteral("focus.task")}) {
        if (auto widget = component(id))
            widget->setEnabled(planEditable);
    }
    if (auto label = qobject_cast<QLabel*>(component("focus.state"))) {
        label->setText(snapshot.state == FocusState::Idle
                           ? QStringLiteral("待开始")
                           : snapshot.state == FocusState::Running
                               ? QStringLiteral("专注中")
                               : snapshot.state == FocusState::Paused
                                   ? QStringLiteral("已暂停")
                                   : snapshot.state == FocusState::SaveFailed
                                       ? QStringLiteral("保存失败")
                                       : QStringLiteral("等待处理"));
    }
}
}
