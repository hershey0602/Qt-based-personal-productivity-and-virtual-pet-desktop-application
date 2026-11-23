#include "TaskPage.h"

namespace fg {
namespace {
QString categoryText(TaskCategory category)
{
    return category == TaskCategory::Study ? QStringLiteral("学习")
         : category == TaskCategory::Work ? QStringLiteral("工作")
                                           : QStringLiteral("生活");
}

QString priorityText(TaskPriority priority)
{
    return priority == TaskPriority::High ? QStringLiteral("高")
         : priority == TaskPriority::Normal ? QStringLiteral("中")
                                             : QStringLiteral("低");
}

QString dueText(const std::optional<QDateTime>& dueAtUtc)
{
    return dueAtUtc ? dueAtUtc->toLocalTime().toString(QStringLiteral("yyyy-MM-dd HH:mm"))
                    : QStringLiteral("未设置");
}

void clearLayout(QLayout* layout)
{
    while (auto item = layout->takeAt(0)) {
        if (auto widget = item->widget())
            widget->deleteLater();
        delete item;
    }
}
}

TaskPage::TaskPage(AppServices& services, QWidget* parent)
    : LayoutPage(services, "tasks", parent)
{
    connect(this, &LayoutPage::actionRequested, this, [this](const QString& action) {
        if (action == QStringLiteral("task.add")) {
            emit newTaskRequested();
        } else if (action == QStringLiteral("task.clear")) {
            if (auto search = qobject_cast<QLineEdit*>(component("tasks.search")))
                search->clear();
            if (auto category = qobject_cast<QComboBox*>(component("tasks.category")))
                category->setCurrentIndex(0);
            if (auto priority = qobject_cast<QComboBox*>(component("tasks.priority")))
                priority->setCurrentIndex(0);
            if (auto status = qobject_cast<QComboBox*>(component("tasks.status")))
                status->setCurrentIndex(0);
        }
    });

    auto search = qobject_cast<QLineEdit*>(component("tasks.search"));
    auto category = qobject_cast<QComboBox*>(component("tasks.category"));
    auto priority = qobject_cast<QComboBox*>(component("tasks.priority"));
    auto status = qobject_cast<QComboBox*>(component("tasks.status"));
    connect(search, &QLineEdit::textChanged, this, [this](const QString& value) {
        filter_.text = value.trimmed();
        refresh();
    });
    connect(category, &QComboBox::currentIndexChanged, this, [this](int index) {
        filter_.category = index == 0 ? std::nullopt
                                      : std::optional<TaskCategory>(TaskCategory(index - 1));
        refresh();
    });
    connect(priority, &QComboBox::currentIndexChanged, this, [this](int index) {
        filter_.priority = index == 0 ? std::nullopt
                                      : std::optional<TaskPriority>(TaskPriority(index - 1));
        refresh();
    });
    connect(status, &QComboBox::currentIndexChanged, this, [this](int index) {
        filter_.state = TaskStateFilter(index);
        refresh();
    });
    refresh();
}

void TaskPage::refresh()
{
    refreshView();
    const auto result = services_.listTasks(filter_);
    if (!result.ok) {
        showResult({false, result.error});
        return;
    }
    renderRows(result.value);
    renderDetails();
}

void TaskPage::selectTask(fg::TaskId id)
{
    const auto result = services_.task(id);
    if (result.ok && result.value && !result.value->deleted)
        selectedTaskId_ = id;
    else
        selectedTaskId_.reset();
    refresh();
}

void TaskPage::renderRows(const QVector<Task>& tasks)
{
    auto container = component("tasks.rows");
    if (!container || !container->layout())
        return;
    clearLayout(container->layout());

    for (const auto& task : tasks) {
        auto row = new QFrame(container);
        row->setProperty("role", selectedTaskId_ && *selectedTaskId_ == task.id
                                   ? "taskSelected"
                                   : task.completed ? "taskDone" : "taskRow");
        row->setProperty("selected", selectedTaskId_ && *selectedTaskId_ == task.id);
        auto layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 8, 0, 8);
        layout->setSpacing(10);

        auto text = new QVBoxLayout;
        auto title = createButton(QStringLiteral("task.select.%1").arg(task.id),
                                  task.title, ButtonRole::Quiet, row);
        title->setMinimumHeight(30);
        title->setMaximumHeight(60);
        title->setProperty("role", task.completed ? "taskDone" : "taskTitle");
        auto meta = new QLabel(QStringLiteral("%1 · 优先级 %2 · 截止 %3")
                                   .arg(categoryText(task.category), priorityText(task.priority),
                                        dueText(task.dueAtUtc)),
                               row);
        meta->setProperty("role", "muted");
        text->addWidget(title);
        text->addWidget(meta);
        layout->addLayout(text, 1);

        auto edit = createButton(QStringLiteral("task.edit.%1").arg(task.id),
                                 QStringLiteral("编辑"), ButtonRole::Quiet, row);
        auto focus = createButton(QStringLiteral("task.focus.%1").arg(task.id),
                                  QStringLiteral("专注"), ButtonRole::Soft, row);
        auto remove = createButton(QStringLiteral("task.delete.%1").arg(task.id),
                                   QStringLiteral("删除"), ButtonRole::Danger, row);
        setSmallButton(*edit);
        setSmallButton(*focus);
        setSmallButton(*remove);
        focus->setEnabled(!task.completed);
        layout->addWidget(edit);
        layout->addWidget(focus);
        layout->addWidget(remove);

        connect(title, &QPushButton::clicked, this, [this, id = task.id] { selectTask(id); });
        connect(edit, &QPushButton::clicked, this, [this, id = task.id] {
            selectTask(id);
            emit editTaskRequested(id);
        });
        connect(focus, &QPushButton::clicked, this, [this, id = task.id] {
            emit focusTaskRequested(id);
        });
        connect(remove, &QPushButton::clicked, this, [this, id = task.id] {
            emit deleteTaskRequested(id);
        });
        container->layout()->addWidget(row);
    }

    if (tasks.isEmpty()) {
        auto empty = new QLabel(QStringLiteral("没有符合当前筛选条件的任务"), container);
        empty->setProperty("role", "muted");
        empty->setAlignment(Qt::AlignCenter);
        container->layout()->addWidget(empty);
    }
}

void TaskPage::renderDetails()
{
    auto container = component("tasks.fields");
    if (!container || !container->layout())
        return;
    clearLayout(container->layout());

    if (!selectedTaskId_) {
        auto empty = new QLabel(QStringLiteral("选择任务后在这里查看详情"), container);
        empty->setProperty("role", "muted");
        empty->setWordWrap(true);
        container->layout()->addWidget(empty);
        return;
    }

    const auto result = services_.task(*selectedTaskId_);
    if (!result.ok || !result.value || result.value->deleted) {
        selectedTaskId_.reset();
        auto empty = new QLabel(QStringLiteral("任务不存在或已删除"), container);
        empty->setProperty("role", "muted");
        container->layout()->addWidget(empty);
        return;
    }

    const auto task = *result.value;
    auto title = new QLabel(task.title, container);
    title->setWordWrap(true);
    title->setProperty("role", "cardTitle");
    auto note = new QLabel(task.note.isEmpty() ? QStringLiteral("无备注") : task.note, container);
    note->setWordWrap(true);
    auto fields = new QLabel(QStringLiteral("分类：%1\n优先级：%2\n状态：%3\n截止：%4")
                                 .arg(categoryText(task.category), priorityText(task.priority),
                                      task.completed ? QStringLiteral("已完成") : QStringLiteral("未完成"),
                                      dueText(task.dueAtUtc)),
                             container);
    fields->setProperty("role", "body");
    fields->setWordWrap(true);
    container->layout()->addWidget(title);
    container->layout()->addWidget(note);
    container->layout()->addWidget(fields);

    auto actions = new QHBoxLayout;
    auto edit = createButton(QStringLiteral("task.detail.edit"), QStringLiteral("编辑"),
                             ButtonRole::Outline, container);
    auto done = createButton(QStringLiteral("task.detail.done"),
                             task.completed ? QStringLiteral("标为未完成") : QStringLiteral("完成"),
                             ButtonRole::Soft, container);
    setSmallButton(*edit);
    setSmallButton(*done);
    actions->addWidget(edit);
    actions->addWidget(done);
    actions->addStretch();
    container->layout()->addItem(actions);
    connect(edit, &QPushButton::clicked, this, [this, id = task.id] { emit editTaskRequested(id); });
    connect(done, &QPushButton::clicked, this, [this, id = task.id, completed = task.completed] {
        showResult(services_.setTaskDone(id, !completed));
        refresh();
    });
}
}
