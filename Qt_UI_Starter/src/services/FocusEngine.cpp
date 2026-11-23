#include "FocusEngine.h"

#include <QUuid>

namespace fg {
namespace {
OperationResult fail(AppErrorCode code, const QString& message)
{
    return {false, {code, message, {}}};
}
}

FocusEngine::FocusEngine(IRepository& repository, ISettingsService& settings, QObject* parent)
    : IFocusEngine(parent), repository_(repository), settings_(settings)
{
    snapshot_.config.minutes = settings_.preferences().defaultFocusMinutes;
    snapshot_.plannedSeconds = snapshot_.config.minutes * 60;
    snapshot_.remainingSeconds = snapshot_.plannedSeconds;
    timer_.setInterval(200);
    connect(&timer_, &QTimer::timeout, this, &FocusEngine::onTick);
}

FocusSnapshot FocusEngine::snapshot() const
{
    FocusSnapshot result = snapshot_;
    if (snapshot_.state == FocusState::Running) {
        result.remainingSeconds = qMax(0, snapshot_.plannedSeconds - activeSeconds());
    }
    return result;
}

std::optional<CompletionResult> FocusEngine::lastCompletion() const { return lastCompletion_; }

OperationResult FocusEngine::configure(const FocusConfig& config)
{
    if (snapshot_.state != FocusState::Idle && snapshot_.state != FocusState::Completed)
        return fail(AppErrorCode::InvalidState, QStringLiteral("当前专注状态不能修改计划"));
    const bool valid = config.mode == FocusMode::Focus
        ? (config.minutes == 25 || config.minutes == 50)
        : config.mode == FocusMode::Custom
            ? (config.minutes >= 15 && config.minutes <= 90 && config.minutes % 5 == 0)
            : config.mode == FocusMode::Rest && (config.minutes == 5 || config.minutes == 10);
    if (!valid) return fail(AppErrorCode::Validation, QStringLiteral("专注时长设置无效"));
    if (config.taskId) {
        auto task = repository_.task(*config.taskId);
        if (!task.ok) return {false, task.error};
        if (!task.value || task.value->deleted || task.value->completed)
            return fail(AppErrorCode::NotFound, QStringLiteral("所选任务不存在或已完成"));
    }
    snapshot_.state = FocusState::Idle;
    snapshot_.sessionId.reset();
    snapshot_.config = config;
    snapshot_.taskTitleSnapshot.clear();
    snapshot_.plannedSeconds = config.minutes * 60;
    snapshot_.remainingSeconds = snapshot_.plannedSeconds;
    publish();
    return {true, {}};
}

OperationResult FocusEngine::prepareNextRound(FocusMode mode)
{
    FocusConfig config;
    config.mode = mode;
    config.durationSource = DurationSource::DefaultSetting;
    if (mode == FocusMode::Rest) config.minutes = settings_.preferences().defaultRestMinutes;
    else config.minutes = settings_.preferences().defaultFocusMinutes;
    config.taskId = snapshot_.config.taskId;
    return configure(config);
}

Result<SessionId> FocusEngine::start()
{
    if (snapshot_.state != FocusState::Idle)
        return {false, {}, {AppErrorCode::InvalidState, QStringLiteral("当前状态不能开始新一轮"), {}}};
    if (snapshot_.config.minutes <= 0)
        return {false, {}, {AppErrorCode::Validation, QStringLiteral("专注时长无效"), {}}};

    QString title;
    if (snapshot_.config.taskId) {
        auto task = repository_.task(*snapshot_.config.taskId);
        if (!task.ok) return {false, {}, task.error};
        if (!task.value || task.value->completed || task.value->deleted)
            return {false, {}, {AppErrorCode::NotFound, QStringLiteral("所选任务已不可用于专注"), {}}};
        title = task.value->title;
    }

    snapshot_.plannedSeconds = snapshot_.config.minutes * 60;
    snapshot_.remainingSeconds = snapshot_.plannedSeconds;
    snapshot_.taskTitleSnapshot = title;
    accumulatedMs_ = 0;
    if (snapshot_.config.mode == FocusMode::Rest) {
        ticking_ = true;
        snapshot_.state = FocusState::Running;
        elapsed_.start();
        timer_.start();
        publish();
        return {true, {}, {}};
    }

    const SessionId id = QUuid::createUuid().toString(QUuid::WithoutBraces).toLower();
    const QDateTime now = QDateTime::currentDateTimeUtc();
    FocusSession session;
    session.sessionId = id;
    session.taskId = snapshot_.config.taskId;
    session.taskTitleSnapshot = title;
    session.mode = snapshot_.config.mode;
    session.plannedSeconds = snapshot_.plannedSeconds;
    session.startedAtUtc = now;
    session.updatedAtUtc = now;
    session.status = SessionStatus::Running;
    session.rewardState = RewardState::NotEligible;
    const OperationResult saved = repository_.insertRunningSession(session);
    if (!saved.ok) return {false, {}, saved.error};

    snapshot_.sessionId = id;
    snapshot_.state = FocusState::Running;
    ticking_ = true;
    elapsed_.start();
    timer_.start();
    publish();
    return {true, id, {}};
}

OperationResult FocusEngine::pause(const SessionId& expectedSessionId)
{
    if (snapshot_.state != FocusState::Running || snapshot_.sessionId != expectedSessionId)
        return fail(AppErrorCode::StaleSession, QStringLiteral("专注会话已变化"));
    const int seconds = activeSeconds();
    if (snapshot_.config.mode != FocusMode::Rest) {
        const auto saved = repository_.updateSessionActivity(
            expectedSessionId, SessionStatus::Paused, seconds, QDateTime::currentDateTimeUtc());
        if (!saved.ok) return saved;
    }
    accumulatedMs_ = seconds * 1000;
    ticking_ = false;
    timer_.stop();
    snapshot_.state = FocusState::Paused;
    snapshot_.remainingSeconds = qMax(0, snapshot_.plannedSeconds - seconds);
    publish();
    return {true, {}};
}

OperationResult FocusEngine::resume(const SessionId& expectedSessionId)
{
    if (snapshot_.state != FocusState::Paused || snapshot_.sessionId != expectedSessionId)
        return fail(AppErrorCode::StaleSession, QStringLiteral("专注会话已变化"));
    if (snapshot_.config.mode != FocusMode::Rest) {
        const auto saved = repository_.updateSessionActivity(
            expectedSessionId, SessionStatus::Running, accumulatedMs_ / 1000,
            QDateTime::currentDateTimeUtc());
        if (!saved.ok) return saved;
    }
    snapshot_.state = FocusState::Running;
    ticking_ = true;
    elapsed_.restart();
    timer_.start();
    publish();
    return {true, {}};
}

OperationResult FocusEngine::abandon(const SessionId& expectedSessionId)
{
    if ((snapshot_.state != FocusState::Running && snapshot_.state != FocusState::Paused)
        || snapshot_.sessionId != expectedSessionId)
        return fail(AppErrorCode::StaleSession, QStringLiteral("专注会话已变化"));
    if (snapshot_.config.mode != FocusMode::Rest) {
        const QDateTime ended = QDateTime::currentDateTimeUtc();
        AbandonWrite write{expectedSessionId,
                           qMin(activeSeconds(), snapshot_.plannedSeconds),
                           ended, ended.toLocalTime().date()};
        const auto saved = repository_.abandonSession(write);
        if (!saved.ok) return saved;
    }
    stopTimer();
    snapshot_.state = FocusState::Idle;
    snapshot_.sessionId.reset();
    snapshot_.remainingSeconds = snapshot_.plannedSeconds;
    snapshot_.taskTitleSnapshot.clear();
    publish();
    return {true, {}};
}

OperationResult FocusEngine::retryCompletionSave(const SessionId& expectedSessionId)
{
    if (snapshot_.state != FocusState::SaveFailed || snapshot_.sessionId != expectedSessionId)
        return fail(AppErrorCode::InvalidState, QStringLiteral("当前没有可重试的完成记录"));
    const auto result = repository_.completeSession(pendingCompletion_);
    if (!result.ok) {
        emit errorOccurred(result.error);
        return result;
    }
    snapshot_.state = FocusState::RewardPending;
    lastCompletion_ = CompletionResult{expectedSessionId, snapshot_.plannedSeconds / 60,
                                       0, RewardState::Pending};
    publish();
    emit completionPersisted(expectedSessionId);
    return {true, {}};
}

void FocusEngine::applySettlementSuccess(const SettlementReceipt& receipt)
{
    if (snapshot_.sessionId != receipt.sessionId) return;
    snapshot_.state = FocusState::Completed;
    if (lastCompletion_) {
        lastCompletion_->coins = receipt.coins;
        lastCompletion_->rewardState = RewardState::Settled;
    }
    publish();
}

void FocusEngine::applySettlementFailure(const SessionId& id, const AppError& error)
{
    if (snapshot_.sessionId == id && snapshot_.state == FocusState::RewardPending)
        emit errorOccurred(error);
}

void FocusEngine::onPreferencesChanged(fg::Preferences preferences)
{
    if (snapshot_.state != FocusState::Idle
        || snapshot_.config.durationSource != DurationSource::DefaultSetting)
        return;
    snapshot_.config.minutes = snapshot_.config.mode == FocusMode::Rest
        ? preferences.defaultRestMinutes : preferences.defaultFocusMinutes;
    snapshot_.plannedSeconds = snapshot_.config.minutes * 60;
    snapshot_.remainingSeconds = snapshot_.plannedSeconds;
    publish();
}

int FocusEngine::activeSeconds() const
{
    if (snapshot_.state == FocusState::Paused) return accumulatedMs_ / 1000;
    if (snapshot_.state != FocusState::Running) return accumulatedMs_ / 1000;
    return qMin(snapshot_.plannedSeconds,
                (accumulatedMs_ + static_cast<int>(elapsed_.elapsed())) / 1000);
}

void FocusEngine::publish()
{
    emit focusChanged(snapshot());
}

void FocusEngine::stopTimer()
{
    timer_.stop();
    ticking_ = false;
    elapsed_.invalidate();
    accumulatedMs_ = 0;
}

void FocusEngine::onTick()
{
    if (!ticking_ || snapshot_.state != FocusState::Running) return;
    const int seconds = activeSeconds();
    snapshot_.remainingSeconds = qMax(0, snapshot_.plannedSeconds - seconds);
    publish();
    if (snapshot_.remainingSeconds == 0) {
        if (snapshot_.config.mode == FocusMode::Rest) {
            stopTimer();
            snapshot_.state = FocusState::Idle;
            snapshot_.remainingSeconds = snapshot_.plannedSeconds;
            emit restFinished();
            publish();
        } else {
            finishNaturally();
        }
    }
}

OperationResult FocusEngine::finishNaturally()
{
    stopTimer();
    const QDateTime ended = QDateTime::currentDateTimeUtc();
    pendingCompletion_ = {snapshot_.sessionId.value_or(QString()),
                          snapshot_.plannedSeconds, ended, ended.toLocalTime().date()};
    const OperationResult saved = repository_.completeSession(pendingCompletion_);
    if (!saved.ok) {
        snapshot_.state = FocusState::SaveFailed;
        snapshot_.remainingSeconds = 0;
        emit errorOccurred(saved.error);
        publish();
        return saved;
    }
    snapshot_.state = FocusState::RewardPending;
    snapshot_.remainingSeconds = 0;
    lastCompletion_ = CompletionResult{*snapshot_.sessionId,
                                       snapshot_.plannedSeconds / 60, 0,
                                       RewardState::Pending};
    publish();
    emit completionPersisted(*snapshot_.sessionId);
    return {true, {}};
}
}
