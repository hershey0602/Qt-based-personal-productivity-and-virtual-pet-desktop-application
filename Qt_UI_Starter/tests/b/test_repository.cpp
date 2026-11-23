#include <QtTest>
#include <QSqlDatabase>

#include "services/Repository.h"

namespace {

fg::FocusSession runningSession(const QString& id, const QDateTime& startedAt)
{
    fg::FocusSession session;
    session.sessionId = id;
    session.taskTitleSnapshot = QStringLiteral("测试专注");
    session.mode = fg::FocusMode::Focus;
    session.plannedSeconds = 1500;
    session.elapsedSeconds = 0;
    session.startedAtUtc = startedAt;
    session.updatedAtUtc = startedAt;
    session.status = fg::SessionStatus::Running;
    session.rewardState = fg::RewardState::NotEligible;
    return session;
}

} // namespace

class RepositoryTests : public QObject
{
    Q_OBJECT

private slots:
    void taskCrudFiltersAndSoftDelete();
    void sessionLifecycleRewardAndStatistics();
    void recoveryAbandonsOnlyInterruptedSessions();
    void feedingIsIdempotentAndAtomic();
};

void RepositoryTests::taskCrudFiltersAndSoftDelete()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    fg::Repository repository;
    const auto opened = repository.open(directory.filePath(QStringLiteral("focusgarden.sqlite")));
    const QString openFailure = QStringLiteral("%1 | %2 | SQL drivers: %3")
                                    .arg(opened.error.message, opened.error.diagnostic,
                                         QSqlDatabase::drivers().join(QStringLiteral(", ")));
    QVERIFY2(opened.ok, qPrintable(openFailure));

    fg::TaskDraft first;
    first.title = QStringLiteral("数学复习");
    first.note = QStringLiteral("只用于备注，不应被标题搜索命中");
    first.category = fg::TaskCategory::Study;
    first.priority = fg::TaskPriority::High;
    first.dueAtUtc = QDateTime(QDate(2026, 11, 22), QTime(9, 0), Qt::UTC);
    const auto createdFirst = repository.createTask(first);
    QVERIFY(createdFirst.ok);
    QVERIFY(createdFirst.value.id > 0);
    QCOMPARE(createdFirst.value.title, QStringLiteral("数学复习"));

    fg::TaskDraft second;
    second.title = QStringLiteral("工作汇报");
    second.category = fg::TaskCategory::Work;
    second.priority = fg::TaskPriority::Normal;
    second.dueAtUtc = QDateTime(QDate(2026, 11, 22), QTime(10, 0), Qt::UTC);
    const auto createdSecond = repository.createTask(second);
    QVERIFY(createdSecond.ok);

    fg::TaskFilter titleFilter;
    titleFilter.text = QStringLiteral("数学");
    const auto titleMatches = repository.listTasks(titleFilter);
    QVERIFY(titleMatches.ok);
    QCOMPARE(titleMatches.value.size(), 1);
    QCOMPARE(titleMatches.value.front().id, createdFirst.value.id);

    fg::TaskFilter noteFilter;
    noteFilter.text = QStringLiteral("备注");
    const auto noteMatches = repository.listTasks(noteFilter);
    QVERIFY(noteMatches.ok);
    QVERIFY(noteMatches.value.isEmpty());

    fg::TaskFilter categoryFilter;
    categoryFilter.category = fg::TaskCategory::Study;
    categoryFilter.state = fg::TaskStateFilter::Incomplete;
    const auto categoryMatches = repository.listTasks(categoryFilter);
    QVERIFY(categoryMatches.ok);
    QCOMPARE(categoryMatches.value.size(), 1);

    QVERIFY(repository.setTaskDone(createdFirst.value.id, true).ok);
    const auto counts = repository.taskCounts();
    QVERIFY(counts.ok);
    QCOMPARE(counts.value.total, qint64(2));
    QCOMPARE(counts.value.completed, qint64(1));

    QVERIFY(repository.softDeleteTask(createdSecond.value.id).ok);
    const auto afterDelete = repository.listTasks({});
    QVERIFY(afterDelete.ok);
    QCOMPARE(afterDelete.value.size(), 1);
    const auto deleted = repository.task(createdSecond.value.id);
    QVERIFY(deleted.ok);
    QVERIFY(deleted.value.has_value());
    QVERIFY(deleted.value->deleted);
    QVERIFY(repository.close().ok);
}

void RepositoryTests::sessionLifecycleRewardAndStatistics()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    fg::Repository repository;
    QVERIFY(repository.open(directory.filePath(QStringLiteral("focusgarden.sqlite"))).ok);

    const QDateTime started(QDate(2026, 11, 22), QTime(8, 0), Qt::UTC);
    const QString sessionId = QStringLiteral("session-reward");
    QVERIFY(repository.insertRunningSession(runningSession(sessionId, started)).ok);
    QVERIFY(repository.updateSessionActivity(
                       sessionId, fg::SessionStatus::Paused, 600, started.addSecs(600))
                .ok);

    fg::CompletionWrite completion;
    completion.sessionId = sessionId;
    completion.elapsedSeconds = 1500;
    completion.endedAtUtc = started.addSecs(1800);
    completion.endedLocalDate = QDate(2026, 11, 22);
    QVERIFY(repository.completeSession(completion).ok);
    QVERIFY(repository.completeSession(completion).ok);

    const auto pending = repository.pendingCompletions();
    QVERIFY(pending.ok);
    QCOMPARE(pending.value.size(), 1);
    QCOMPARE(pending.value.front().rewardState, fg::RewardState::Pending);

    fg::RewardCreditRequest request;
    request.sessionId = sessionId;
    request.expectedCoins = 20;
    const auto firstSettlement = repository.creditRewardAtomically(request);
    QVERIFY(firstSettlement.ok);
    QVERIFY(firstSettlement.value.newlyCredited);
    QCOMPARE(firstSettlement.value.coins, qint64(20));
    QCOMPARE(firstSettlement.value.pet.coins, qint64(20));

    const auto repeatedSettlement = repository.creditRewardAtomically(request);
    QVERIFY(repeatedSettlement.ok);
    QVERIFY(!repeatedSettlement.value.newlyCredited);
    QCOMPARE(repeatedSettlement.value.coins, qint64(20));
    QCOMPARE(repeatedSettlement.value.pet.coins, qint64(20));

    const auto savedSession = repository.session(sessionId);
    QVERIFY(savedSession.ok);
    QVERIFY(savedSession.value.has_value());
    QCOMPARE(savedSession.value->status, fg::SessionStatus::Completed);
    QCOMPARE(savedSession.value->rewardState, fg::RewardState::Settled);

    const auto statistics = repository.statistics(QDate(2026, 11, 22));
    QVERIFY(statistics.ok);
    QCOMPARE(statistics.value.todaySeconds, qint64(1500));
    QCOMPARE(statistics.value.totalSeconds, qint64(1500));
    QCOMPARE(statistics.value.last7Days.size(), 7);
    QCOMPARE(statistics.value.last7Days.back().date, QDate(2026, 11, 22));
    QCOMPARE(statistics.value.last7Days.back().seconds, qint64(1500));
    QCOMPARE(statistics.value.history.size(), 1);
    QCOMPARE(statistics.value.history.front().coins, qint64(20));
    QVERIFY(repository.close().ok);
}

void RepositoryTests::recoveryAbandonsOnlyInterruptedSessions()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    fg::Repository repository;
    QVERIFY(repository.open(directory.filePath(QStringLiteral("focusgarden.sqlite"))).ok);

    const QDateTime started(QDate(2026, 11, 21), QTime(23, 30), Qt::UTC);
    QVERIFY(repository.insertRunningSession(runningSession(QStringLiteral("session-recover"), started)).ok);
    QVERIFY(repository.updateSessionActivity(
                       QStringLiteral("session-recover"), fg::SessionStatus::Paused, 300,
                       started.addSecs(300))
                .ok);

    const QDateTime recovered(QDate(2026, 11, 22), QTime(8, 0), Qt::UTC);
    QVERIFY(repository.recoverInterruptedSessions(recovered).ok);
    const auto recoveredSession = repository.session(QStringLiteral("session-recover"));
    QVERIFY(recoveredSession.ok);
    QVERIFY(recoveredSession.value.has_value());
    QCOMPARE(recoveredSession.value->status, fg::SessionStatus::Abandoned);
    QCOMPARE(recoveredSession.value->elapsedSeconds, 300);
    QVERIFY(recoveredSession.value->endedAtUtc.has_value());
    QCOMPARE(recoveredSession.value->endedLocalDate.value(), QDate(2026, 11, 22));
    QCOMPARE(repository.pendingCompletions().value.size(), 0);
    QVERIFY(repository.close().ok);
}

void RepositoryTests::feedingIsIdempotentAndAtomic()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    fg::Repository repository;
    QVERIFY(repository.open(directory.filePath(QStringLiteral("focusgarden.sqlite"))).ok);

    const QDateTime started(QDate(2026, 11, 22), QTime(8, 0), Qt::UTC);
    QVERIFY(repository.insertRunningSession(runningSession(QStringLiteral("session-feed"), started)).ok);
    fg::CompletionWrite completion;
    completion.sessionId = QStringLiteral("session-feed");
    completion.elapsedSeconds = 1500;
    completion.endedAtUtc = started.addSecs(1500);
    completion.endedLocalDate = QDate(2026, 11, 22);
    QVERIFY(repository.completeSession(completion).ok);

    fg::RewardCreditRequest reward;
    reward.sessionId = QStringLiteral("session-feed");
    reward.expectedCoins = 20;
    QVERIFY(repository.creditRewardAtomically(reward).ok);

    fg::FeedCommand command;
    command.operationId = QStringLiteral("feed-operation-1");
    const auto firstFeed = repository.feedPetAtomically(command);
    QVERIFY(firstFeed.ok);
    QVERIFY(firstFeed.value.newlyApplied);
    QCOMPARE(firstFeed.value.coinsSpent, qint64(15));
    QCOMPARE(firstFeed.value.moodAdded, 15);
    QCOMPARE(firstFeed.value.pet.coins, qint64(5));
    QCOMPARE(firstFeed.value.pet.cumulativeExp, qint64(10));

    const auto repeatedFeed = repository.feedPetAtomically(command);
    QVERIFY(repeatedFeed.ok);
    QVERIFY(!repeatedFeed.value.newlyApplied);
    QCOMPARE(repeatedFeed.value.pet.coins, qint64(5));
    QCOMPARE(repeatedFeed.value.pet.cumulativeExp, qint64(10));

    fg::FeedCommand insufficient;
    insufficient.operationId = QStringLiteral("feed-operation-2");
    const auto failedFeed = repository.feedPetAtomically(insufficient);
    QVERIFY(!failedFeed.ok);
    QCOMPARE(failedFeed.error.code, fg::AppErrorCode::InsufficientCoins);

    const auto petAfterFailure = repository.petSnapshot();
    QVERIFY(petAfterFailure.ok);
    QCOMPARE(petAfterFailure.value.coins, qint64(5));
    QCOMPARE(petAfterFailure.value.cumulativeExp, qint64(10));
    QCOMPARE(petAfterFailure.value.mood, 85);
    QVERIFY(repository.close().ok);
}

QTEST_MAIN(RepositoryTests)
#include "test_repository.moc"
