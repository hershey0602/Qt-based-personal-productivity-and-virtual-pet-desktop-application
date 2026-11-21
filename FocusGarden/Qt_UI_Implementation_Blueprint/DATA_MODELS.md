# 共享数据结构和类型冻结

版本 FG-API-1.0。本文件代码块是下一阶段需要生成的声明规范，不是已编译头文件。目标路径为 Qt_UI_Starter/src/contracts/DomainTypes.h；A唯一维护，B/C/D不得自行定义同名替代结构。包含 QtCore/QString、QStringList、QDate、QDateTime、QVector、QMetaType、QtGlobal 与 C++17 optional。

```cpp
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
```

## 语义不变量

1. ok=true时error.code必须None；ok=false时value不供页面使用，error必须含中文message。写入失败不发成功通知，不关闭需要重试的模态框。Result<std::optional<Task>>成功但value为空，表示未找到；与数据库失败明确区分。
2. title用Unicode码点计数（如toUcs4().size），trim后1～100；note最多500。时间戳全部以UTC ISO-8601含毫秒写库；选择日期在UI转UTC，渲染时转本地。结束本地日期在首次自然结束时固化，重试不得改到重试当天。
3. TaskId由SQLite整数主键生成；SessionId/OperationId由创建会话/喂食命令的一方生成UUID。恢复或重试沿用原ID，不能重新生成再写一次。
4. 非休息自然完成的elapsedSeconds固定为plannedSeconds（活动计时达到计划即结束，GUI晚刷新不多计）。暂停时间不计入；放弃不计有效统计。QElapsedTimer累计毫秒，QTimer只刷新；不能按tick次数扣秒。
5. 休息只在FocusEngine内计时，不进入有效专注仓库和金币账本，结束或放弃后回Idle。数据库sessions仅存Focus/Custom；这沿用已交付原型与PRD的休息不产生有效专注规则。
6. 金币公式每5分钟4币；喂食15币、+10EXP、心情+15上限100。等级由累计EXP推导：<50为1、50～149为2、>=150为3；满级继续喂食，隐藏升级条。
7. 页面不得保留第二份业务钱包/任务表；可以缓存上次渲染的快照，但刷新后以服务为准。音乐和任务筛选属于当前应用状态，不因切页重置。
8. ModelMode::Demo的假数据只来自MockPorts/测试夹具；Live首次空数据库必须任务0、记录0、金币0、宠物EXP0/心情70。不能把原型seed写进正式数据库。

## Qt元类型

A在生成DomainTypes.h时，对出现在信号参数中的自定义值类型使用Q_DECLARE_METATYPE并集中qRegisterMetaType。包括PageId、TaskId/SessionId的基础类型及FocusConfig、FocusSnapshot、Preferences、MusicPreferences、MusicSnapshot、PetSnapshot、SettlementReceipt、FeedReceipt、CompletionResult、AppError、NoticeLevel；别名基础类型不重复声明同一个Qt元类型。模板Result不直接跨信号传输。信号统一按本包SIGNAL_SLOT_MATRIX的值参数声明，避免四个人各加const引用造成连接签名分歧。

阶段约束：前5天完成全部PRD必做功能与真实持久化；第1天Mock仅用于支架并行，日5验收不接受演示替代。第6天后只测试、修复、优化、部署与答辩准备。

模式校验：Focus仅25或50分钟，Custom为15～90分钟且5分钟步长，Rest仅5或10分钟；任务在start时再次校验未完成/未删除。库中仅保存Focus/Custom记录。
