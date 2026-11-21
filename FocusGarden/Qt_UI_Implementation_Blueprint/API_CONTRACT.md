# 唯一共享接口契约

版本 FG-API-1.0。覆盖旧版API中的简写；代码生成时唯一依据是本文件与DATA_MODELS.md。这里冻结的是声明和协作约定，尚未生成或编译Qt工程，不宣称接口已通过编译验证。G-01已由队长确认：仅搜索标题；本版无未决的搜索产品选项。

## 谁定义 谁实现 谁调用

A将本文件接口一次生成到 Qt_UI_Starter/src/contracts/ServicePorts.h、DomainTypes.h，并维护版本。B/C/D只实现分配给自己的具体类，不另建AppServices、不复制公共模型。I前缀是抽象服务端口，所有页面只依赖AppServices；Repository不接收业务服务引用。

下方每个签名均位于namespace fg。抽象服务继承QObject，含Q_OBJECT，构造使用QObject(parent)，虚析构默认；仅I前缀端口的方法在ServicePorts.h中声明为virtual纯虚；AppServices与CompletionCoordinator是A的具体类，签名写到各自头文件且实现一次，不再次生成同名抽象类。I前缀端口的signals原样声明在对应抽象基类，B/C/D具体实现不重复声明；A的AppServices和CompletionCoordinator在各自具体类声明下表signals一次。表中给出的const、引用、optional与Result均不可自行删改。onPreferencesChanged与Coordinator::settle声明为public slots，其余命令为public方法。

## IRepository

具体实现：Repository；实现负责人：B；仅依赖：Qt/存储基础设施，无其他业务服务。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `OperationResult open(const QString& databasePath);` | A装配时一次调用；启用外键，建表/迁移；失败不进入Live |
| `OperationResult close();` | A关闭时调用；先停止计时/播放器并销毁查询对象 |
| `Result<QVector<Task>> listTasks(const TaskFilter& filter) const;` | TaskManager，排除软删并稳定排序 |
| `Result<std::optional<Task>> task(TaskId id) const;` | TaskManager/RewardService（不可读取软删任务作为新会话） |
| `Result<Task> createTask(const TaskDraft& draft);` | TaskManager，单事务 |
| `Result<Task> updateTask(TaskId id, const TaskDraft& draft);` | TaskManager，保留完成状态与创建时间 |
| `OperationResult setTaskDone(TaskId id, bool completed);` | TaskManager，零金币 |
| `OperationResult softDeleteTask(TaskId id);` | TaskManager，只标deleted |
| `Result<QVector<Task>> todayTasks(const QDate& date, int limit) const;` | StatisticsService；同日截止、升序、最多3 |
| `Result<TaskCounts> taskCounts() const;` | StatisticsService；全部未删除任务 |
| `OperationResult insertRunningSession(const FocusSession& session);` | FocusEngine；仅Focus/Custom，在开始成功后启动计时 |
| `OperationResult updateSessionActivity(const SessionId& id, SessionStatus status, int elapsedSeconds, const QDateTime& updatedAtUtc);` | FocusEngine；只允许Running/Paused更新 |
| `OperationResult completeSession(const CompletionWrite& write);` | FocusEngine；首次写Completed/Pending；相同完成值幂等 |
| `OperationResult abandonSession(const AbandonWrite& write);` | FocusEngine；仅Running/Paused可转Abandoned，零币 |
| `Result<std::optional<FocusSession>> session(const SessionId& id) const;` | RewardService/Coordinator只读 |
| `Result<QVector<FocusSession>> pendingCompletions() const;` | Coordinator启动恢复：Completed且Pending |
| `OperationResult recoverInterruptedSessions(const QDateTime& recoveredAtUtc);` | 启动由A调用；遗留Running/Paused标Abandoned，不补奖 |
| `Result<StatisticsSnapshot> statistics(const QDate& anchorDate) const;` | StatisticsService统一计算首页与统计口径 |
| `Result<PetSnapshot> petSnapshot() const;` | PetManager/RewardService；余额SUM，不另建wallet |
| `Result<SettlementReceipt> creditRewardAtomically(const RewardCreditRequest& request);` | C RewardService调用；B执行事务及唯一索引防重 |
| `Result<FeedReceipt> feedPetAtomically(const FeedCommand& command);` | C PetManager调用；B原子扣币+经验+心情 |

信号声明：

```cpp
// 此端口没有业务信号；写入成功由上层服务统一发布。
```

## ITaskService

具体实现：TaskManager；实现负责人：B；仅依赖：IRepository。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `Result<QVector<Task>> listTasks(const TaskFilter& filter) const;` | A/B页面经AppServices调用 |
| `Result<std::optional<Task>> task(TaskId id) const;` | 共用编辑弹窗与详情 |
| `Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id);` | 空id新增；有id编辑；共同校验 |
| `OperationResult setTaskDone(TaskId id, bool completed);` | 完成恢复，不发币 |
| `OperationResult softDeleteTask(TaskId id);` | 确认后软删 |

信号声明：

```cpp
void tasksChanged();
void errorOccurred(fg::AppError error);
```

## IStatisticsService

具体实现：StatisticsService；实现负责人：B；仅依赖：IRepository。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `Result<StatisticsSnapshot> statistics() const;` | A/B页面调用，以当前本地日为anchor |
| `Result<QVector<Task>> todayTasks() const;` | A今日最多3项 |

信号声明：

```cpp
// 此端口没有业务信号；写入成功由上层服务统一发布。
```

## ISettingsService

具体实现：SettingsService；实现负责人：D；仅依赖：Qt/存储基础设施，无其他业务服务。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `OperationResult load();` | 启动一次；损坏/失败明确报错，内存采用默认值但不冒称已保存 |
| `Preferences preferences() const;` | B/A读取已加载缓存 |
| `OperationResult savePreferences(const Preferences& preferences);` | D设置页经门面调用；sync/status失败恢复旧缓存和旧键并报错 |
| `MusicPreferences musicPreferences() const;` | D MusicController读取清单与音量 |
| `OperationResult saveMusicPreferences(const MusicPreferences& preferences);` | D MusicController保存本地路径与音量，不新增设置页选项 |

信号声明：

```cpp
void preferencesChanged(fg::Preferences preferences);
void musicPreferencesChanged(fg::MusicPreferences preferences);
void errorOccurred(fg::AppError error);
```

## IFocusEngine

具体实现：FocusEngine；实现负责人：B；仅依赖：IRepository, ISettingsService。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `FocusSnapshot snapshot() const;` | 只读快照 |
| `std::optional<CompletionResult> lastCompletion() const;` | 最近结果，包括Pending；切页不清空 |
| `OperationResult configure(const FocusConfig& config);` | 仅Idle/Completed可配置；不开始；Completed先转Idle但保留lastCompletion |
| `OperationResult prepareNextRound(FocusMode mode);` | 读取D当前默认，DefaultSetting来源，转Idle，不启动 |
| `Result<SessionId> start();` | 只允许Idle；生成UUID与标题快照；保存Running成功才开始 |
| `OperationResult pause(const SessionId& expectedSessionId);` | 只允许同会话Running，保存状态失败不伪报暂停 |
| `OperationResult resume(const SessionId& expectedSessionId);` | 只允许同会话Paused |
| `OperationResult abandon(const SessionId& expectedSessionId);` | 确认后调用；持久化失败保留原活动状态；休息只清内存 |
| `OperationResult retryCompletionSave(const SessionId& expectedSessionId);` | 仅当前SaveFailed；保留原结束时间/日期/ID |
| `void applySettlementSuccess(const SettlementReceipt& receipt);` | 仅A Coordinator调用；当前同id转Completed；老记录只更新结果 |
| `void applySettlementFailure(const SessionId& id, const AppError& error);` | 仅Coordinator；已保存记录保留Pending，不回退Completed数据库状态 |
| `void onPreferencesChanged(fg::Preferences preferences);` | Qt slot；仅Idle且DefaultSetting更新草稿，活动/显式时长不变 |

信号声明：

```cpp
void focusChanged(fg::FocusSnapshot snapshot);
void completionPersisted(fg::SessionId sessionId);
void restFinished();
void errorOccurred(fg::AppError error);
```

## IRewardService

具体实现：RewardService；实现负责人：C；仅依赖：IRepository。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `Result<SettlementReceipt> settleCompletedSession(const SessionId& sessionId);` | 仅Coordinator调用；读已保存完成记录，算奖励，调Repo原子端口 |

信号声明：

```cpp
void rewardCommitted(fg::SettlementReceipt receipt);
void errorOccurred(fg::AppError error);
```

## IPetService

具体实现：PetManager；实现负责人：C；仅依赖：IRepository。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `Result<PetSnapshot> pet() const;` | A/C页面共享快照 |
| `Result<FeedReceipt> feed();` | 生成一个OperationId，C规则校验，调用Repo原子喂食 |

信号声明：

```cpp
void petChanged(fg::PetSnapshot pet);
void petLeveled(fg::FeedReceipt receipt);
void errorOccurred(fg::AppError error);
```

## IMusicController

具体实现：MusicController；实现负责人：D；仅依赖：ISettingsService。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `OperationResult loadPlaylist();` | 启动仅恢复路径/音量，不自动播放 |
| `MusicSnapshot snapshot() const;` | MusicPage只读 |
| `OperationResult importFiles(const QStringList& absolutePaths);` | 一次多选本地文件；保存失败保持原清单 |
| `OperationResult select(int index);` | 单击选中；换曲停止并归零，不自动播 |
| `OperationResult playPause();` | 命令被接受不代表已播放；以后端状态信号为准 |
| `OperationResult stop();` | 停止归零 |
| `OperationResult previous();` | 顺序循环；正在播放时切曲保持播放 |
| `OperationResult next();` | 同上；媒体自然结束强制播放后继 |
| `OperationResult seek(qint64 positionMs);` | 仅seekable允许；限制到0..duration |
| `OperationResult setVolume(int volume);` | 0..100；实时生效并保存，失败报告 |
| `OperationResult remove(int index);` | 仅移除引用；非当前曲维持播放；当前曲停止并选相邻合法项 |

信号声明：

```cpp
void musicChanged(fg::MusicSnapshot snapshot);
void errorOccurred(fg::AppError error);
```

## ICompletionSound

具体实现：CompletionSound；实现负责人：D；仅依赖：ISettingsService。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `void playForCompletedSession(const SessionId& sessionId);` | A连接B已保存自然完成信号；读取开关，按sessionId抑制重复音效 |

信号声明：

```cpp
void errorOccurred(fg::AppError error);
```

## CompletionCoordinator

具体实现：CompletionCoordinator；实现负责人：A；仅依赖：IFocusEngine, IRewardService, IRepository。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `void settle(fg::SessionId sessionId);` | Qt slot；接B completionPersisted，调用C一次结算 |
| `OperationResult retry(const SessionId& sessionId);` | SaveFailed先B重试保存；Pending仅C重试结算 |
| `OperationResult retryPending();` | 启动恢复后/进入Focus枚举已保存未结算记录；失败不循环轰炸提示 |

信号声明：

```cpp
void settlementCompleted(fg::SettlementReceipt receipt);
void settlementFailed(fg::SessionId sessionId, fg::AppError error);
```

## AppServices

具体实现：AppServices；实现负责人：A；仅依赖：ITaskService, IStatisticsService, IFocusEngine, IPetService, ISettingsService, IMusicController, CompletionCoordinator。

| 准确 C++ 签名 | 调用者与语义 |
|---|---|
| `Result<QVector<Task>> listTasks(const TaskFilter& filter) const;` | 转B TaskManager |
| `Result<std::optional<Task>> task(TaskId id) const;` | 转B |
| `Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id);` | 转B；无默认参数省略，新增显式std::nullopt |
| `OperationResult setTaskDone(TaskId id, bool completed);` | 转B |
| `OperationResult softDeleteTask(TaskId id);` | 转B |
| `Result<QVector<Task>> todayTasks() const;` | 转B StatisticsService |
| `Result<StatisticsSnapshot> statistics() const;` | 转B StatisticsService |
| `FocusSnapshot focus() const;` | 转B snapshot |
| `std::optional<CompletionResult> lastCompletion() const;` | 转B |
| `OperationResult configureFocus(const FocusConfig& config);` | 转B configure |
| `OperationResult preselectFocusTask(TaskId id);` | 查任务后构造D默认时长配置；活动时返回冲突，A仍导航Focus |
| `OperationResult prepareNextRound(FocusMode mode);` | 转B；不自动开始 |
| `Result<SessionId> startFocus();` | 转B start |
| `OperationResult pauseFocus(const SessionId& expectedSessionId);` | 转B pause |
| `OperationResult resumeFocus(const SessionId& expectedSessionId);` | 转B resume |
| `OperationResult abandonFocus(const SessionId& expectedSessionId);` | 转B abandon |
| `OperationResult retryFocusCompletion(const SessionId& sessionId);` | 转A Coordinator；禁止UI自己改金币 |
| `Result<PetSnapshot> pet() const;` | 转C PetManager |
| `Result<FeedReceipt> feedPet();` | 转C feed |
| `Preferences preferences() const;` | 转D SettingsService |
| `OperationResult savePreferences(const Preferences& preferences);` | 转D |
| `IMusicController& music() const;` | 返回同一D播放器非拥有引用 |
| `ModelMode modelMode() const;` | 只读Demo/Live标识 |

信号声明：

```cpp
void dataChanged();
void focusChanged(fg::FocusSnapshot snapshot);
void preferencesChanged(fg::Preferences preferences);
void musicChanged(fg::MusicSnapshot snapshot);
void focusCompleted(fg::CompletionResult result);
void petLeveled(fg::FeedReceipt receipt);
void notice(QString message, fg::NoticeLevel level);
```

## 具体类构造和生命周期

```cpp
Repository(QObject* parent = nullptr);
TaskManager(IRepository& repository, QObject* parent = nullptr);
StatisticsService(IRepository& repository, QObject* parent = nullptr);
SettingsService(const QString& settingsFilePath, QObject* parent = nullptr);
FocusEngine(IRepository& repository, ISettingsService& settings, QObject* parent = nullptr);
RewardService(IRepository& repository, QObject* parent = nullptr);
PetManager(IRepository& repository, QObject* parent = nullptr);
MusicController(ISettingsService& settings, QObject* parent = nullptr);
CompletionSound(ISettingsService& settings, QObject* parent = nullptr);
CompletionCoordinator(IFocusEngine& focus, IRewardService& rewards,
                      IRepository& repository, QObject* parent = nullptr);
AppServices(ITaskService& tasks, IStatisticsService& statistics,
            IFocusEngine& focus, IPetService& pet, ISettingsService& settings,
            IMusicController& music, CompletionCoordinator& completion,
            ModelMode mode, QObject* parent = nullptr);
```

AppComposition（A）是应用级QObject，以上对象均以它为parent，仅创建一次；MainWindow和页面保存非拥有引用。先创建Repository/Settings，再其他服务、Coordinator、AppServices，最后MainWindow和七页。页面的parent属于QStackedWidget/QScrollArea；切换只setCurrentWidget。

## 保存 结算 失败与退出

B自然完成先冻结结束时间和elapsed，Repository::completeSession成功后才发completionPersisted。A Coordinator接信号调C settleCompletedSession。C只读完成记录并通过Repo原子端口写账；C不直接持有FocusEngine，B不持有RewardService，从而没有相互依赖。Coordinator先调用B applySettlementSuccess，再发settlementCompleted；失败先调用B applySettlementFailure再发settlementFailed。AppServices两个接收槽仅刷新/提示，不再次调用applySettlement。新收入才发focusCompleted；幂等返回newlyCredited=false不二次发金币或弹新到账。

SaveFailed只重试B的写入，不能调C；RewardPending已保存只重试C，不能重写新会话或改结束日期。Pending结算持久化后可正常关闭程序并启动恢复；不能把奖励暂时失败误作记录未保存。SaveFailed未保存时正常关闭需先重试保存；重试失败保留窗口和错误，不虚报保存。

活动Running/Paused关闭时，ConfirmDialog捕获原sessionId；确认调abandonFocus(expectedId)，保存成功才退出；取消不改计时。若确认期间已自然完成，不能把Completed覆写成Abandoned。恢复启动时Running/Paused历史标Abandoned，不补奖；Completed/Pending走retryPending。

## 设置与播放器

SettingsService（D）唯一写app.ini中的三项用户设置及内部music路径/音量；音乐内部键不成为设置页新增选项。QSettings指定IniFormat、固定完整路径；sync/status失败返回Persistence并恢复旧内存配置，错误不得发preferencesChanged成功信号。

B收到preferencesChanged仅在Idle且durationSource=DefaultSetting时更新尚未开始的草稿；Running/Paused/SaveFailed/Pending均不改当前计划。Completed后的prepareNextRound和从任务预选读取D已保存的新值。显式选择25/50/自定义应优先于默认值，不因导航覆盖。

MusicController持有一个QMediaPlayer和QAudioOutput；CompletionSound持有独立QSoundEffect，只播放提供的短提示音，不控制歌曲。媒体命令返回ok表示请求被接受；播放、时长与错误以后台musicChanged/错误信号为准。单击换曲不自动播，双击由select()+playPause()触发；文件失效停止/跳过且每次操作最多一条错误，不能无限重试坏文件。

## 统一错误和刷新

没有跨接口C++异常；所有失败使用Result/OperationResult。页面校验先提示，服务仍要再次校验。页面不得向Repository发写请求。dataChanged仅由AppServices聚合tasksChanged、Coordinator::settlementCompleted、petChanged等成功事件发出；不再订阅rewardCommitted重复刷新，失败不发成功；A将全页刷新排到GUI事件队列，避免正在点击的行被同步删除。focusChanged只更新专注计时/状态，不每tick重建全部页面。错误日志含diagnostic，用户只看message。

## 日1模拟与真实切换

A创建MockPorts覆盖全部端口，读取返回空数据/初始宠物/默认设置，未实现写入返回NotImplemented并标明演示；不替成员实现业务。所有真实具体类先有可编译占位，B/C/D接管各自同名文件。演示或Live只在启动装配时选一次；切页不切后端。成员写自己的服务与独立测试后，由A在AppComposition替换相应Mock；不用改页面接口，不另建工程。

阶段约束：前5天完成全部PRD必做功能与真实持久化；第1天Mock仅用于支架并行，日5验收不接受演示替代。第6天后只测试、修复、优化、部署与答辩准备。

自然完成的同步回调顺序：B完成写库成功，先将当前snapshot置RewardPending并发focusChanged，再发completionPersisted；A可能同步结算成功回调B，因此B不能在该信号返回后再次覆写旧状态。
