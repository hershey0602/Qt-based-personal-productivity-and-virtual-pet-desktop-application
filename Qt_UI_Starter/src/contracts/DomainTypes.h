#pragma once
#include <QtCore>
#include <optional>
namespace fg {
using TaskId = qint64;       // 有效ID > 0；可空值使用std::optional，不以-1代替
using LedgerId = qint64;
using SessionId = QString;  // QUuid::WithoutBraces生成，小写UUID，持久化后永不重用
using OperationId = QString;
using Coins = qint64;

enum class PageId : int { Dashboard=0, Tasks=1, Focus=2, Pet=3,
                          Statistics=4, Music=5, Settings=6 };
enum class TaskCategory : int { Study=0, Work=1, Life=2 };
enum class TaskPriority : int { High=0, Normal=1, Low=2 };
enum class TaskStateFilter : int { All=0, Incomplete=1, Completed=2 };
enum class FocusMode : int { Focus=0, Custom=1, Rest=2 };
enum class DurationSource : int { DefaultSetting=0, ExplicitSelection=1 };
enum class FocusState : int { Idle=0, Running=1, Paused=2,
                              SaveFailed=3, RewardPending=4, Completed=5 };
enum class SessionStatus : int { Running=0, Paused=1, Completed=2, Abandoned=3 };
enum class RewardState : int { NotEligible=0, Pending=1, Settled=2 };
enum class PlaybackState : int { Stopped=0, Playing=1, Paused=2 };
enum class NoticeLevel : int { Success=0, Info=1, Error=2 };
enum class ModelMode : int { Demo=0, Live=1 };
enum class AppErrorCode : int { None=0, Validation=1, NotFound=2,
 ActiveSessionConflict=3, InvalidState=4, Persistence=5,
 SettlementPending=6, InsufficientCoins=7, FileMissing=8,
 MediaUnsupported=9, NotSeekable=10, NotImplemented=11, StaleSession=12 };

struct AppError {
 AppErrorCode code=AppErrorCode::None;
 QString message;       // 给用户看的中文错误，不包含SQL/内部路径
 QString diagnostic;    // 只写日志，不直接放进产品界面
};
struct OperationResult { bool ok=false; AppError error; };
template<class T> struct Result { bool ok=false; T value{}; AppError error; };

struct TaskDraft {
 QString title;
 QString note;
 TaskCategory category=TaskCategory::Study;
 TaskPriority priority=TaskPriority::Normal;
 std::optional<QDateTime> dueAtUtc;
};
struct Task {
 TaskId id=0;
 QString title;
 QString note;
 TaskCategory category=TaskCategory::Study;
 TaskPriority priority=TaskPriority::Normal;
 std::optional<QDateTime> dueAtUtc;
 QDateTime createdAtUtc;
 bool completed=false;
 bool deleted=false;
};
struct TaskFilter {
 QString text;  // 仅匹配标题，队长已确认；不匹配备注
 std::optional<TaskCategory> category;
 std::optional<TaskPriority> priority;
 TaskStateFilter state=TaskStateFilter::All;
};
struct TaskCounts { qint64 total=0; qint64 completed=0; };
struct Preferences {
 int defaultFocusMinutes=25;  // 只能25或50
 int defaultRestMinutes=5;    // 只能5或10
 bool completionSoundEnabled=true;
};
struct MusicPreferences { QStringList localPaths; int volume=70; };
struct FocusConfig {
 std::optional<TaskId> taskId;
 FocusMode mode=FocusMode::Focus;
 int minutes=25;
 DurationSource durationSource=DurationSource::DefaultSetting;
};
struct FocusSnapshot {
 FocusState state=FocusState::Idle;
 std::optional<SessionId> sessionId;
 FocusConfig config;
 QString taskTitleSnapshot;
 int plannedSeconds=1500;
 int remainingSeconds=1500;
};
struct FocusSession {
 SessionId sessionId;
 std::optional<TaskId> taskId;
 QString taskTitleSnapshot;
 FocusMode mode=FocusMode::Focus;
 int plannedSeconds=1500;
 int elapsedSeconds=0;
 QDateTime startedAtUtc;
 std::optional<QDateTime> endedAtUtc;
 std::optional<QDate> endedLocalDate;
 QDateTime updatedAtUtc;
 SessionStatus status=SessionStatus::Running;
 RewardState rewardState=RewardState::NotEligible;
};
struct CompletionWrite {
 SessionId sessionId;
 int elapsedSeconds=0;
 QDateTime endedAtUtc;
 QDate endedLocalDate;
};
struct AbandonWrite {
 SessionId sessionId;
 int elapsedSeconds=0;
 QDateTime endedAtUtc;
 QDate endedLocalDate;
};
struct PetSnapshot {
 Coins coins=0;                 // 来源是coin_ledger合计，不是第二份钱包
 qint64 cumulativeExp=0;
 int mood=70;
 int level=1;                   // 由EXP推导，不单独持久化等级
};
struct RewardCreditRequest { SessionId sessionId; Coins expectedCoins=0; };
struct SettlementReceipt {
 SessionId sessionId;
 Coins coins=0;                 // 此会话收入金额，即使重复查询也保持原金额
 bool newlyCredited=false;      // false代表已经结算，不能再弹一次“新到账”
 PetSnapshot pet;
};
struct FeedCommand {
 OperationId operationId;
 Coins cost=15;
 qint64 expGain=10;
 int moodGain=15;
};
struct FeedReceipt {
 OperationId operationId;
 bool newlyApplied=false;
 Coins coinsSpent=15;
 qint64 expAdded=10;
 int moodAdded=0;               // 饱和到100后的实际增加量
 int levelBefore=1;
 int levelAfter=1;
 PetSnapshot pet;
};
struct CompletionResult {
 SessionId sessionId;
 int minutes=0;
 Coins coins=0;
 RewardState rewardState=RewardState::Pending;
};
struct DailyFocus { QDate date; qint64 seconds=0; };
struct SessionHistoryRow { FocusSession session; Coins coins=0; };
struct StatisticsSnapshot {
 qint64 todaySeconds=0;
 qint64 totalSeconds=0;
 TaskCounts tasks;
 QVector<DailyFocus> last7Days;  // 固定7项，升序，包含今天，缺失日补0
 QVector<SessionHistoryRow> history;  // 结束时间降序
};
struct MusicTrack { QString absolutePath; QString displayName; bool available=true; };
struct MusicSnapshot {
 QVector<MusicTrack> tracks;
 std::optional<int> currentIndex;
 PlaybackState state=PlaybackState::Stopped;
 qint64 positionMs=0;
 qint64 durationMs=0;
 bool seekable=false;
 int volume=70;
};
}

Q_DECLARE_METATYPE(fg::PageId)
Q_DECLARE_METATYPE(fg::FocusConfig)
Q_DECLARE_METATYPE(fg::FocusSnapshot)
Q_DECLARE_METATYPE(fg::Preferences)
Q_DECLARE_METATYPE(fg::MusicPreferences)
Q_DECLARE_METATYPE(fg::MusicSnapshot)
Q_DECLARE_METATYPE(fg::PetSnapshot)
Q_DECLARE_METATYPE(fg::SettlementReceipt)
Q_DECLARE_METATYPE(fg::FeedReceipt)
Q_DECLARE_METATYPE(fg::CompletionResult)
Q_DECLARE_METATYPE(fg::AppError)
Q_DECLARE_METATYPE(fg::NoticeLevel)
namespace fg { inline void registerMetaTypes() {qRegisterMetaType<PageId>();qRegisterMetaType<FocusConfig>();qRegisterMetaType<FocusSnapshot>();qRegisterMetaType<Preferences>();qRegisterMetaType<MusicPreferences>();qRegisterMetaType<MusicSnapshot>();qRegisterMetaType<PetSnapshot>();qRegisterMetaType<SettlementReceipt>();qRegisterMetaType<FeedReceipt>();qRegisterMetaType<CompletionResult>();qRegisterMetaType<AppError>();qRegisterMetaType<NoticeLevel>();} }
