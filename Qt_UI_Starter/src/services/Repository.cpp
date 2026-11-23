#include "Repository.h"

#include "storage/Migrations.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QDate>

namespace fg {

namespace {

bool execSql(QSqlDatabase& database, const QString& sql, QString* diagnostic = nullptr)
{
    QSqlQuery query(database);
    if (query.exec(sql)) {
        return true;
    }
    if (diagnostic) {
        *diagnostic = query.lastError().text();
    }
    return false;
}

QString localDate(const QDateTime& utc)
{
    return utc.toUTC().toLocalTime().date().toString(Qt::ISODate);
}

} // namespace

Repository::Repository(QObject* parent)
    : IRepository(parent)
    , m_connectionName(QStringLiteral("fg.repository.%1")
                           .arg(QUuid::createUuid().toString(QUuid::WithoutBraces)))
{
}

Repository::~Repository()
{
    close();
}

AppError Repository::error(AppErrorCode code, const QString& message, const QString& diagnostic) const
{
    return {code, message, diagnostic};
}

OperationResult Repository::failure(AppErrorCode code, const QString& message,
                                    const QString& diagnostic) const
{
    return {false, error(code, message, diagnostic)};
}

bool Repository::ready(QString* diagnostic) const
{
    if (m_open && m_database.isValid() && m_database.isOpen()) {
        return true;
    }
    if (diagnostic) {
        *diagnostic = QStringLiteral("Repository database is not open");
    }
    return false;
}

QString Repository::timestamp(const QDateTime& value)
{
    return value.toUTC().toString(Qt::ISODateWithMs);
}

QDateTime Repository::parseTimestamp(const QVariant& value)
{
    return QDateTime::fromString(value.toString(), Qt::ISODate).toUTC();
}

std::optional<QDate> Repository::parseDate(const QVariant& value)
{
    const QDate date = QDate::fromString(value.toString(), Qt::ISODate);
    return date.isValid() ? std::optional<QDate>(date) : std::nullopt;
}

QString Repository::escapeLike(const QString& value)
{
    QString escaped = value;
    escaped.replace('\\', QStringLiteral("\\\\"));
    escaped.replace('%', QStringLiteral("\\%"));
    escaped.replace('_', QStringLiteral("\\_"));
    return escaped;
}

int Repository::levelForExp(qint64 exp)
{
    return exp >= 150 ? 3 : (exp >= 50 ? 2 : 1);
}

bool Repository::validTaskDraft(const TaskDraft& draft, QString* message) const
{
    const int titleLength = draft.title.trimmed().toUcs4().size();
    if (titleLength < 1 || titleLength > 100) {
        if (message) *message = QStringLiteral("任务标题需为1到100个字符");
        return false;
    }
    if (draft.note.toUcs4().size() > 500) {
        if (message) *message = QStringLiteral("任务备注不能超过500个字符");
        return false;
    }
    const int category = static_cast<int>(draft.category);
    const int priority = static_cast<int>(draft.priority);
    if (category < 0 || category > 2 || priority < 0 || priority > 2) {
        if (message) *message = QStringLiteral("任务分类或优先级无效");
        return false;
    }
    return true;
}

Task Repository::taskFromQuery(const QSqlQuery& query) const
{
    Task task;
    task.id = query.value(0).toLongLong();
    task.title = query.value(1).toString();
    task.note = query.value(2).toString();
    task.category = static_cast<TaskCategory>(query.value(3).toInt());
    task.priority = static_cast<TaskPriority>(query.value(4).toInt());
    if (!query.value(5).isNull()) task.dueAtUtc = parseTimestamp(query.value(5));
    task.createdAtUtc = parseTimestamp(query.value(6));
    task.completed = query.value(7).toInt() != 0;
    task.deleted = query.value(8).toInt() != 0;
    return task;
}

FocusSession Repository::sessionFromQuery(const QSqlQuery& query) const
{
    FocusSession value;
    value.sessionId = query.value(0).toString();
    if (!query.value(1).isNull()) value.taskId = query.value(1).toLongLong();
    value.taskTitleSnapshot = query.value(2).toString();
    value.mode = static_cast<FocusMode>(query.value(3).toInt());
    value.plannedSeconds = query.value(4).toInt();
    value.elapsedSeconds = query.value(5).toInt();
    value.startedAtUtc = parseTimestamp(query.value(6));
    if (!query.value(7).isNull()) value.endedAtUtc = parseTimestamp(query.value(7));
    value.endedLocalDate = parseDate(query.value(8));
    value.updatedAtUtc = parseTimestamp(query.value(9));
    value.status = static_cast<SessionStatus>(query.value(10).toInt());
    value.rewardState = static_cast<RewardState>(query.value(11).toInt());
    return value;
}

OperationResult Repository::open(const QString& databasePath)
{
    if (m_open) {
        const OperationResult closed = close();
        if (!closed.ok) return closed;
    }
    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_database.setDatabaseName(databasePath);
    if (!m_database.open()) {
        const QString diagnostic = m_database.lastError().text();
        close();
        return failure(AppErrorCode::Persistence, QStringLiteral("数据库打开失败"), diagnostic);
    }
    QString diagnostic;
    if (!Migrations::apply(m_database, &diagnostic)) {
        close();
        return failure(AppErrorCode::Persistence, QStringLiteral("数据库初始化失败"), diagnostic);
    }
    m_open = true;
    return {true, {}};
}

OperationResult Repository::close()
{
    if (m_database.isValid()) {
        m_database.close();
        m_database = QSqlDatabase();
        QSqlDatabase::removeDatabase(m_connectionName);
    }
    m_open = false;
    return {true, {}};
}

OperationResult Repository::insertRunningSession(const FocusSession& value)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    if (value.sessionId.trimmed().isEmpty() || value.mode == FocusMode::Rest
        || (value.mode != FocusMode::Focus && value.mode != FocusMode::Custom)
        || value.plannedSeconds < 900 || value.plannedSeconds > 5400 || value.plannedSeconds % 300
        || value.elapsedSeconds < 0 || value.elapsedSeconds > value.plannedSeconds
        || !value.startedAtUtc.isValid() || !value.updatedAtUtc.isValid()
        || (value.status != SessionStatus::Running && value.status != SessionStatus::Paused)
        || value.rewardState != RewardState::NotEligible) {
        return failure(AppErrorCode::Validation, QStringLiteral("专注记录数据无效"));
    }
    if (!m_database.transaction()) return failure(AppErrorCode::Persistence, QStringLiteral("无法开始数据库事务"), m_database.lastError().text());
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO sessions(session_id,task_id,task_title_snapshot,mode,planned_seconds,elapsed_seconds,"
        "started_at_utc,updated_at_utc,status,reward_state) "
        "VALUES(:id,:task,:title,:mode,:planned,:elapsed,:started,:updated,:status,:reward)"));
    query.bindValue(":id", value.sessionId);
    query.bindValue(":task", value.taskId ? QVariant(*value.taskId) : QVariant());
    query.bindValue(":title", value.taskTitleSnapshot);
    query.bindValue(":mode", static_cast<int>(value.mode));
    query.bindValue(":planned", value.plannedSeconds);
    query.bindValue(":elapsed", value.elapsedSeconds);
    query.bindValue(":started", timestamp(value.startedAtUtc));
    query.bindValue(":updated", timestamp(value.updatedAtUtc));
    query.bindValue(":status", static_cast<int>(value.status));
    query.bindValue(":reward", static_cast<int>(value.rewardState));
    if (!query.exec() || !m_database.commit()) {
        diagnostic = query.lastError().isValid() ? query.lastError().text() : m_database.lastError().text();
        m_database.rollback();
        return failure(AppErrorCode::Persistence, QStringLiteral("专注记录保存失败"), diagnostic);
    }
    return {true, {}};
}

OperationResult Repository::updateSessionActivity(const SessionId& id, SessionStatus status,
                                                   int elapsedSeconds, const QDateTime& updatedAtUtc)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    if ((status != SessionStatus::Running && status != SessionStatus::Paused)
        || elapsedSeconds < 0 || !updatedAtUtc.isValid())
        return failure(AppErrorCode::Validation, QStringLiteral("专注状态数据无效"));
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "UPDATE sessions SET status=:status,elapsed_seconds=:elapsed,updated_at_utc=:updated "
        "WHERE session_id=:id AND status IN(0,1) AND elapsed_seconds<=planned_seconds AND :elapsed<=planned_seconds"));
    query.bindValue(":status", static_cast<int>(status));
    query.bindValue(":elapsed", elapsedSeconds);
    query.bindValue(":updated", timestamp(updatedAtUtc));
    query.bindValue(":id", id);
    if (!query.exec()) return failure(AppErrorCode::Persistence, QStringLiteral("专注状态保存失败"), query.lastError().text());
    if (query.numRowsAffected() == 0) {
        auto current = session(id);
        if (!current.ok) return {false, current.error};
        if (!current.value) return failure(AppErrorCode::NotFound, QStringLiteral("专注记录不存在"));
        return failure(AppErrorCode::StaleSession, QStringLiteral("专注记录状态已变化"));
    }
    return {true, {}};
}

OperationResult Repository::completeSession(const CompletionWrite& write)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    if (write.sessionId.isEmpty() || write.elapsedSeconds < 0 || !write.endedAtUtc.isValid()
        || !write.endedLocalDate.isValid())
        return failure(AppErrorCode::Validation, QStringLiteral("完成记录数据无效"));
    if (!m_database.transaction()) return failure(AppErrorCode::Persistence, QStringLiteral("无法开始数据库事务"), m_database.lastError().text());
    QSqlQuery read(m_database);
    read.prepare("SELECT status,planned_seconds,elapsed_seconds,ended_at_utc,ended_local_date FROM sessions WHERE session_id=:id");
    read.bindValue(":id", write.sessionId);
    if (!read.exec() || !read.next()) {
        diagnostic = read.lastError().text();
        m_database.rollback();
        return failure(read.lastError().isValid() ? AppErrorCode::Persistence : AppErrorCode::NotFound,
                       QStringLiteral("专注记录不存在或读取失败"), diagnostic);
    }
    const auto status = static_cast<SessionStatus>(read.value(0).toInt());
    const int planned = read.value(1).toInt();
    const bool sameCompletion = status == SessionStatus::Completed
        && read.value(2).toInt() == planned && write.elapsedSeconds == planned
        && parseTimestamp(read.value(3)) == write.endedAtUtc.toUTC()
        && read.value(4).toString() == write.endedLocalDate.toString(Qt::ISODate);
    if (sameCompletion) {
        m_database.commit();
        return {true, {}};
    }
    if ((status != SessionStatus::Running && status != SessionStatus::Paused)
        || write.elapsedSeconds != planned) {
        m_database.rollback();
        return failure(AppErrorCode::StaleSession, QStringLiteral("专注记录不能完成或已被其他操作修改"));
    }
    QSqlQuery update(m_database);
    update.prepare("UPDATE sessions SET status=2,reward_state=1,elapsed_seconds=:elapsed,ended_at_utc=:ended,ended_local_date=:date,updated_at_utc=:ended WHERE session_id=:id AND status IN(0,1)");
    update.bindValue(":elapsed", write.elapsedSeconds);
    update.bindValue(":ended", timestamp(write.endedAtUtc));
    update.bindValue(":date", write.endedLocalDate.toString(Qt::ISODate));
    update.bindValue(":id", write.sessionId);
    if (!update.exec() || update.numRowsAffected() != 1 || !m_database.commit()) {
        diagnostic = update.lastError().isValid() ? update.lastError().text() : m_database.lastError().text();
        m_database.rollback();
        return failure(AppErrorCode::Persistence, QStringLiteral("专注完成记录保存失败"), diagnostic);
    }
    return {true, {}};
}

OperationResult Repository::abandonSession(const AbandonWrite& write)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    if (!write.endedAtUtc.isValid() || !write.endedLocalDate.isValid() || write.elapsedSeconds < 0)
        return failure(AppErrorCode::Validation, QStringLiteral("放弃记录数据无效"));
    QSqlQuery query(m_database);
    query.prepare("UPDATE sessions SET status=3,reward_state=0,elapsed_seconds=min(:elapsed,planned_seconds),ended_at_utc=:ended,ended_local_date=:date,updated_at_utc=:ended WHERE session_id=:id AND status IN(0,1) AND :elapsed<=planned_seconds");
    query.bindValue(":elapsed", write.elapsedSeconds);
    query.bindValue(":ended", timestamp(write.endedAtUtc));
    query.bindValue(":date", write.endedLocalDate.toString(Qt::ISODate));
    query.bindValue(":id", write.sessionId);
    if (!query.exec()) return failure(AppErrorCode::Persistence, QStringLiteral("放弃记录保存失败"), query.lastError().text());
    if (!query.numRowsAffected()) return failure(AppErrorCode::StaleSession, QStringLiteral("专注记录状态已变化"));
    return {true, {}};
}

Result<std::optional<FocusSession>> Repository::session(const SessionId& id) const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    QSqlQuery query(m_database);
    query.prepare("SELECT session_id,task_id,task_title_snapshot,mode,planned_seconds,elapsed_seconds,started_at_utc,ended_at_utc,ended_local_date,updated_at_utc,status,reward_state FROM sessions WHERE session_id=:id");
    query.bindValue(":id", id);
    if (!query.exec()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("专注记录读取失败"), query.lastError().text())};
    if (!query.next()) return {true, std::nullopt, {}};
    return {true, std::optional<FocusSession>(sessionFromQuery(query)), {}};
}

Result<QVector<FocusSession>> Repository::pendingCompletions() const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    QSqlQuery query(m_database);
    if (!query.exec("SELECT session_id,task_id,task_title_snapshot,mode,planned_seconds,elapsed_seconds,started_at_utc,ended_at_utc,ended_local_date,updated_at_utc,status,reward_state FROM sessions WHERE status=2 AND reward_state=1 ORDER BY ended_at_utc ASC"))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("待结算记录读取失败"), query.lastError().text())};
    QVector<FocusSession> values;
    while (query.next()) values.push_back(sessionFromQuery(query));
    return {true, values, {}};
}

OperationResult Repository::recoverInterruptedSessions(const QDateTime& recoveredAtUtc)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    if (!recoveredAtUtc.isValid()) return failure(AppErrorCode::Validation, QStringLiteral("恢复时间无效"));
    QSqlQuery query(m_database);
    query.prepare("UPDATE sessions SET status=3,reward_state=0,ended_at_utc=:ended,ended_local_date=:date,updated_at_utc=:ended WHERE status IN(0,1)");
    query.bindValue(":ended", timestamp(recoveredAtUtc));
    query.bindValue(":date", localDate(recoveredAtUtc));
    if (!query.exec()) return failure(AppErrorCode::Persistence, QStringLiteral("中断记录恢复失败"), query.lastError().text());
    return {true, {}};
}

Result<StatisticsSnapshot> Repository::statistics(const QDate& anchorDate) const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    if (!anchorDate.isValid()) return {false, {}, error(AppErrorCode::Validation, QStringLiteral("统计日期无效"))};
    StatisticsSnapshot snapshot;
    snapshot.last7Days.reserve(7);
    const QDate first = anchorDate.addDays(-6);
    for (int i = 0; i < 7; ++i) snapshot.last7Days.push_back({first.addDays(i), 0});
    QSqlQuery totals(m_database);
    totals.prepare("SELECT COALESCE(SUM(CASE WHEN ended_local_date=:today THEN elapsed_seconds ELSE 0 END),0),COALESCE(SUM(elapsed_seconds),0) FROM sessions WHERE status=2 AND mode IN(0,1) AND ended_local_date<=:today");
    totals.bindValue(":today", anchorDate.toString(Qt::ISODate));
    if (!totals.exec() || !totals.next()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("专注统计读取失败"), totals.lastError().text())};
    snapshot.todaySeconds = totals.value(0).toLongLong();
    snapshot.totalSeconds = totals.value(1).toLongLong();
    QSqlQuery days(m_database);
    days.prepare("SELECT ended_local_date,SUM(elapsed_seconds) FROM sessions WHERE status=2 AND mode IN(0,1) AND ended_local_date BETWEEN :first AND :today GROUP BY ended_local_date");
    days.bindValue(":first", first.toString(Qt::ISODate));
    days.bindValue(":today", anchorDate.toString(Qt::ISODate));
    if (!days.exec()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("每日统计读取失败"), days.lastError().text())};
    while (days.next()) {
        const QDate date = QDate::fromString(days.value(0).toString(), Qt::ISODate);
        const int index = first.daysTo(date);
        if (index >= 0 && index < 7) snapshot.last7Days[index].seconds = days.value(1).toLongLong();
    }
    const auto counts = taskCounts();
    if (!counts.ok) return {false, {}, counts.error};
    snapshot.tasks = counts.value;
    QSqlQuery history(m_database);
    if (!history.exec("SELECT s.session_id,s.task_id,s.task_title_snapshot,s.mode,s.planned_seconds,s.elapsed_seconds,s.started_at_utc,s.ended_at_utc,s.ended_local_date,s.updated_at_utc,s.status,s.reward_state,COALESCE(l.delta,0) FROM sessions s LEFT JOIN coin_ledger l ON l.session_id=s.session_id AND l.reason='focus_reward' WHERE s.status IN(2,3) ORDER BY s.ended_at_utc DESC"))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("专注历史读取失败"), history.lastError().text())};
    while (history.next()) snapshot.history.push_back({sessionFromQuery(history), history.value(12).toLongLong()});
    return {true, snapshot, {}};
}

PetSnapshot Repository::petSnapshotUnlocked(QString* diagnostic) const
{
    PetSnapshot pet;
    QSqlQuery query(m_database);
    if (!query.exec("SELECT COALESCE((SELECT SUM(delta) FROM coin_ledger),0),(SELECT cumulative_exp FROM pet_state WHERE singleton_id=1),(SELECT mood FROM pet_state WHERE singleton_id=1)")
        || !query.next()) {
        if (diagnostic) *diagnostic = query.lastError().text();
        return pet;
    }
    pet.coins = query.value(0).toLongLong();
    pet.cumulativeExp = query.value(1).toLongLong();
    pet.mood = query.value(2).toInt();
    pet.level = levelForExp(pet.cumulativeExp);
    return pet;
}

Result<PetSnapshot> Repository::petSnapshot() const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    PetSnapshot pet = petSnapshotUnlocked(&diagnostic);
    if (!diagnostic.isEmpty()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("宠物数据读取失败"), diagnostic)};
    return {true, pet, {}};
}

Result<SettlementReceipt> Repository::creditRewardAtomically(const RewardCreditRequest& request)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    if (!m_database.transaction()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("无法开始结算事务"), m_database.lastError().text())};
    QSqlQuery sessionQuery(m_database);
    sessionQuery.prepare("SELECT status,mode,planned_seconds,elapsed_seconds,reward_state FROM sessions WHERE session_id=:id");
    sessionQuery.bindValue(":id", request.sessionId);
    if (!sessionQuery.exec() || !sessionQuery.next()) {
        diagnostic = sessionQuery.lastError().text();
        m_database.rollback();
        return {false, {}, error(AppErrorCode::NotFound, QStringLiteral("专注记录不存在"), diagnostic)};
    }
    const int planned = sessionQuery.value(2).toInt();
    const Coins amount = static_cast<Coins>(planned / 300) * 4;
    if (sessionQuery.value(0).toInt() != static_cast<int>(SessionStatus::Completed)
        || sessionQuery.value(3).toInt() != planned
        || sessionQuery.value(1).toInt() == static_cast<int>(FocusMode::Rest)
        || request.expectedCoins != amount) {
        m_database.rollback();
        return {false, {}, error(AppErrorCode::Validation, QStringLiteral("该专注记录不符合奖励结算条件"))};
    }
    QSqlQuery existing(m_database);
    existing.prepare("SELECT delta FROM coin_ledger WHERE session_id=:id AND reason='focus_reward'");
    existing.bindValue(":id", request.sessionId);
    if (!existing.exec()) {
        diagnostic = existing.lastError().text(); m_database.rollback();
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("奖励流水读取失败"), diagnostic)};
    }
    bool newlyCredited = false;
    Coins credited = amount;
    if (existing.next()) {
        credited = existing.value(0).toLongLong();
        if (sessionQuery.value(4).toInt() != static_cast<int>(RewardState::Settled)) {
            QSqlQuery mark(m_database);
            mark.prepare("UPDATE sessions SET reward_state=2 WHERE session_id=:id AND status=2");
            mark.bindValue(":id", request.sessionId);
            if (!mark.exec() || !mark.numRowsAffected()) {
                diagnostic = mark.lastError().text(); m_database.rollback();
                return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("奖励状态修复失败"), diagnostic)};
            }
        }
    } else {
        QSqlQuery insert(m_database);
        insert.prepare("INSERT INTO coin_ledger(operation_id,session_id,reason,delta,created_at_utc) VALUES(:op,:id,'focus_reward',:delta,:created)");
        insert.bindValue(":op", QStringLiteral("reward:") + request.sessionId);
        insert.bindValue(":id", request.sessionId);
        insert.bindValue(":delta", amount);
        insert.bindValue(":created", timestamp(QDateTime::currentDateTimeUtc()));
        if (!insert.exec()) {
            diagnostic = insert.lastError().text(); m_database.rollback();
            return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("奖励流水写入失败"), diagnostic)};
        }
        QSqlQuery mark(m_database);
        mark.prepare("UPDATE sessions SET reward_state=2 WHERE session_id=:id AND status=2");
        mark.bindValue(":id", request.sessionId);
        if (!mark.exec() || mark.numRowsAffected() != 1) {
            diagnostic = mark.lastError().text(); m_database.rollback();
            return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("奖励状态更新失败"), diagnostic)};
        }
        newlyCredited = true;
    }
    if (!m_database.commit()) {
        diagnostic = m_database.lastError().text(); m_database.rollback();
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("奖励结算提交失败"), diagnostic)};
    }
    PetSnapshot pet = petSnapshotUnlocked(&diagnostic);
    if (!diagnostic.isEmpty()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("宠物数据读取失败"), diagnostic)};
    return {true, {request.sessionId, credited, newlyCredited, pet}, {}};
}

Result<FeedReceipt> Repository::feedPetAtomically(const FeedCommand& command)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    if (command.operationId.isEmpty() || command.cost != 15 || command.expGain != 10 || command.moodGain != 15)
        return {false, {}, error(AppErrorCode::Validation, QStringLiteral("喂食操作数据无效"))};
    if (!m_database.transaction()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("无法开始喂食事务"), m_database.lastError().text())};
    QSqlQuery prior(m_database);
    prior.prepare("SELECT delta,exp_before,exp_after,mood_before,mood_after FROM coin_ledger WHERE operation_id=:id AND reason='feed'");
    prior.bindValue(":id", command.operationId);
    if (!prior.exec()) {
        diagnostic = prior.lastError().text(); m_database.rollback();
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("喂食流水查询失败"), diagnostic)};
    }
    if (prior.next()) {
        const qint64 expBefore = prior.value(1).toLongLong();
        const qint64 expAfter = prior.value(2).toLongLong();
        const int moodBefore = prior.value(3).toInt();
        const int moodAfter = prior.value(4).toInt();
        if (!m_database.commit()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("喂食重试提交失败"), m_database.lastError().text())};
        PetSnapshot pet = petSnapshotUnlocked(&diagnostic);
        if (!diagnostic.isEmpty()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("宠物数据读取失败"), diagnostic)};
        return {true, {command.operationId, false, 15, 10, moodAfter - moodBefore,
                       levelForExp(expBefore), levelForExp(expAfter), pet}, {}};
    }
    PetSnapshot before = petSnapshotUnlocked(&diagnostic);
    if (!diagnostic.isEmpty()) {
        m_database.rollback();
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("宠物数据读取失败"), diagnostic)};
    }
    if (before.coins < command.cost) {
        m_database.rollback();
        return {false, {}, error(AppErrorCode::InsufficientCoins, QStringLiteral("金币不足，无法喂食"))};
    }
    const qint64 expAfter = before.cumulativeExp + command.expGain;
    const int moodAfter = qMin(100, before.mood + command.moodGain);
    QSqlQuery insert(m_database);
    insert.prepare("INSERT INTO coin_ledger(operation_id,reason,delta,created_at_utc,exp_before,exp_after,mood_before,mood_after) VALUES(:id,'feed',-15,:created,:eb,:ea,:mb,:ma)");
    insert.bindValue(":id", command.operationId);
    insert.bindValue(":created", timestamp(QDateTime::currentDateTimeUtc()));
    insert.bindValue(":eb", before.cumulativeExp);
    insert.bindValue(":ea", expAfter);
    insert.bindValue(":mb", before.mood);
    insert.bindValue(":ma", moodAfter);
    QSqlQuery update(m_database);
    update.prepare("UPDATE pet_state SET cumulative_exp=:exp,mood=:mood WHERE singleton_id=1");
    update.bindValue(":exp", expAfter);
    update.bindValue(":mood", moodAfter);
    if (!insert.exec() || !update.exec() || update.numRowsAffected() != 1 || !m_database.commit()) {
        diagnostic = insert.lastError().isValid() ? insert.lastError().text()
            : (update.lastError().isValid() ? update.lastError().text() : m_database.lastError().text());
        m_database.rollback();
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("喂食事务写入失败"), diagnostic)};
    }
    PetSnapshot after = petSnapshotUnlocked(&diagnostic);
    if (!diagnostic.isEmpty()) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("宠物数据读取失败"), diagnostic)};
    return {true, {command.operationId, true, command.cost, command.expGain, moodAfter - before.mood,
                   levelForExp(before.cumulativeExp), levelForExp(expAfter), after}, {}};
}

Result<QVector<Task>> Repository::listTasks(const TaskFilter& filter) const
{
    QString diagnostic;
    if (!ready(&diagnostic))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};

    QString sql = QStringLiteral(
        "SELECT id,title,note,category,priority,due_at_utc,created_at_utc,completed,deleted "
        "FROM tasks WHERE deleted=0");
    if (!filter.text.trimmed().isEmpty()) sql += QStringLiteral(" AND title LIKE :text ESCAPE '\\'");
    if (filter.category) sql += QStringLiteral(" AND category=:category");
    if (filter.priority) sql += QStringLiteral(" AND priority=:priority");
    if (filter.state == TaskStateFilter::Incomplete) sql += QStringLiteral(" AND completed=0");
    if (filter.state == TaskStateFilter::Completed) sql += QStringLiteral(" AND completed=1");
    sql += QStringLiteral(
        " ORDER BY due_at_utc IS NULL ASC,due_at_utc ASC,created_at_utc DESC,id DESC");

    QSqlQuery query(m_database);
    query.prepare(sql);
    if (!filter.text.trimmed().isEmpty())
        query.bindValue(QStringLiteral(":text"), QStringLiteral("%") + escapeLike(filter.text.trimmed()) + "%");
    if (filter.category) query.bindValue(QStringLiteral(":category"), static_cast<int>(*filter.category));
    if (filter.priority) query.bindValue(QStringLiteral(":priority"), static_cast<int>(*filter.priority));
    if (!query.exec())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("任务查询失败"), query.lastError().text())};

    QVector<Task> tasks;
    while (query.next()) tasks.push_back(taskFromQuery(query));
    return {true, tasks, {}};
}

Result<std::optional<Task>> Repository::task(TaskId id) const
{
    QString diagnostic;
    if (!ready(&diagnostic))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT id,title,note,category,priority,due_at_utc,created_at_utc,completed,deleted "
        "FROM tasks WHERE id=:id"));
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("任务读取失败"), query.lastError().text())};
    if (!query.next()) return {true, std::nullopt, {}};
    return {true, std::optional<Task>(taskFromQuery(query)), {}};
}

Result<Task> Repository::createTask(const TaskDraft& draft)
{
    QString message;
    if (!validTaskDraft(draft, &message))
        return {false, {}, error(AppErrorCode::Validation, message)};
    QString diagnostic;
    if (!ready(&diagnostic))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO tasks(title,note,category,priority,due_at_utc,created_at_utc) "
        "VALUES(:title,:note,:category,:priority,:due,:created)"));
    query.bindValue(QStringLiteral(":title"), draft.title.trimmed());
    query.bindValue(QStringLiteral(":note"),
                    draft.note.isNull() ? QStringLiteral("") : draft.note);
    query.bindValue(QStringLiteral(":category"), static_cast<int>(draft.category));
    query.bindValue(QStringLiteral(":priority"), static_cast<int>(draft.priority));
    query.bindValue(QStringLiteral(":due"), draft.dueAtUtc ? timestamp(*draft.dueAtUtc) : QVariant());
    query.bindValue(QStringLiteral(":created"), timestamp(QDateTime::currentDateTimeUtc()));
    if (!query.exec())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("任务保存失败"), query.lastError().text())};
    const qint64 id = query.lastInsertId().toLongLong();
    auto saved = task(id);
    if (!saved.ok) return {false, {}, saved.error};
    if (!saved.value) {
        return {false, {},
                error(AppErrorCode::Persistence, QStringLiteral("任务已保存但无法读取"),
                      QStringLiteral("Inserted task id: %1").arg(id))};
    }
    return {true, *saved.value, {}};
}

Result<Task> Repository::updateTask(TaskId id, const TaskDraft& draft)
{
    QString message;
    if (!validTaskDraft(draft, &message))
        return {false, {}, error(AppErrorCode::Validation, message)};
    QString diagnostic;
    if (!ready(&diagnostic))
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "UPDATE tasks SET title=:title,note=:note,category=:category,priority=:priority,due_at_utc=:due "
        "WHERE id=:id AND deleted=0"));
    query.bindValue(QStringLiteral(":title"), draft.title.trimmed());
    query.bindValue(QStringLiteral(":note"),
                    draft.note.isNull() ? QStringLiteral("") : draft.note);
    query.bindValue(QStringLiteral(":category"), static_cast<int>(draft.category));
    query.bindValue(QStringLiteral(":priority"), static_cast<int>(draft.priority));
    query.bindValue(QStringLiteral(":due"), draft.dueAtUtc ? timestamp(*draft.dueAtUtc) : QVariant());
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("任务更新失败"), query.lastError().text())};
    auto saved = task(id);
    if (!saved.ok) return {false, {}, saved.error};
    if (!saved.value || saved.value->deleted)
        return {false, {}, error(AppErrorCode::NotFound, QStringLiteral("任务不存在"))};
    return {true, *saved.value, {}};
}

OperationResult Repository::setTaskDone(TaskId id, bool completed)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("UPDATE tasks SET completed=:value WHERE id=:id AND deleted=0"));
    query.bindValue(QStringLiteral(":value"), completed ? 1 : 0);
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec()) return failure(AppErrorCode::Persistence, QStringLiteral("任务状态更新失败"), query.lastError().text());
    if (query.numRowsAffected() == 0) {
        auto found = task(id);
        if (!found.ok) return {false, found.error};
        if (!found.value || found.value->deleted) return failure(AppErrorCode::NotFound, QStringLiteral("任务不存在"));
    }
    return {true, {}};
}

OperationResult Repository::softDeleteTask(TaskId id)
{
    QString diagnostic;
    if (!ready(&diagnostic)) return failure(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic);
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("UPDATE tasks SET deleted=1 WHERE id=:id AND deleted=0"));
    query.bindValue(QStringLiteral(":id"), id);
    if (!query.exec()) return failure(AppErrorCode::Persistence, QStringLiteral("任务删除失败"), query.lastError().text());
    if (query.numRowsAffected() == 0) return failure(AppErrorCode::NotFound, QStringLiteral("任务不存在或已删除"));
    return {true, {}};
}

Result<QVector<Task>> Repository::todayTasks(const QDate& date, int limit) const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    if (!date.isValid() || limit <= 0)
        return {false, {}, error(AppErrorCode::Validation, QStringLiteral("日期或数量无效"))};
    const QDateTime startLocal(date, QTime(0, 0), Qt::LocalTime);
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT id,title,note,category,priority,due_at_utc,created_at_utc,completed,deleted "
        "FROM tasks WHERE deleted=0 AND due_at_utc>=:start AND due_at_utc<:end "
        "ORDER BY due_at_utc ASC,created_at_utc DESC,id DESC LIMIT :limit"));
    query.bindValue(QStringLiteral(":start"), timestamp(startLocal.toUTC()));
    query.bindValue(QStringLiteral(":end"), timestamp(startLocal.addDays(1).toUTC()));
    query.bindValue(QStringLiteral(":limit"), limit);
    if (!query.exec())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("今日任务查询失败"), query.lastError().text())};
    QVector<Task> tasks;
    while (query.next()) tasks.push_back(taskFromQuery(query));
    return {true, tasks, {}};
}

Result<TaskCounts> Repository::taskCounts() const
{
    QString diagnostic;
    if (!ready(&diagnostic)) return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("数据库未打开"), diagnostic)};
    QSqlQuery query(m_database);
    if (!query.exec(QStringLiteral(
            "SELECT COUNT(*),COALESCE(SUM(CASE WHEN completed=1 THEN 1 ELSE 0 END),0) "
            "FROM tasks WHERE deleted=0")) || !query.next())
        return {false, {}, error(AppErrorCode::Persistence, QStringLiteral("任务统计查询失败"), query.lastError().text())};
    return {true, {query.value(0).toLongLong(), query.value(1).toLongLong()}, {}};
}

} // namespace fg
