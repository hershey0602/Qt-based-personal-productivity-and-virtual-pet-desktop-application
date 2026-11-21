# B_任务专注统计与数据库_完整任务书

你负责任务 专注 统计 SQLite数据库。这是从开工到最后交付的一次性完整任务。前5天实现全部分配功能与真实持久化，后续只测试修复优化部署答辩；Mock仅日1支架/测试替身。SettingsPage与SettingsService由D实现，A只装配与统一UI。功能以有效PRD、视觉以UI V2.1、接口以FG-API-1.0为准。

## 1 我负责什么

| 编号 | 范围 | 必做内容 |
|---|---|---|
| B01 | 任务和共用编辑器 | 新增/编辑/完成/恢复/软删除；标题trim后1～100码点，备注<=500，学习/工作/生活，高/中/低，截止可空可过去。新增编辑同TaskEditDialog；dirty取消/X/Esc二层确认保留输入。 |
| B02 | 查询筛选与详情 | 仅标题实时搜索，分类/优先级/状态AND叠加；截止升序、空截止最后、相同截止创建时间倒序。点击整行只读详情；跨页保留筛选和选中。已完成可编辑删除恢复但不新开专注。 |
| B03 | 唯一SQLite仓库 | 实现FG-API全部IRepository端口和schema迁移，唯一连接；任务/会话读写及供C调用的奖励/喂食原子方法。重启保留数据，正式库不注入演示样例。 |
| B04 | 真实计时 | 25/50，Custom15～90步长5，Rest5/10；手动开始、暂停、继续、确认放弃、自然完成；QElapsedTimer活动毫秒，QTimer只刷新；离页和最小化继续。 |
| B05 | 会话与奖励边界 | 开始保存唯一SessionId及标题快照；保存Completed成功才发completionPersisted，A协调C；保存失败不发币，结算失败保留Completed/Pending，重试保留原ID/结束日。 |
| B06 | 任务冲突与异常恢复 | 活动任务修改/完成/删除不打断现轮，历史用快照；另一任务专注入口保留原会话；放弃保存失败保留计时；关闭确认期间自然完成不可覆写；异常重启遗留Running/Paused转Abandoned零币。 |
| B07 | 统计真实口径 | 今日/累计只计Completed活动秒，未结算仍计时长；已完成任务/全量未删除完成率，0分母显示暂无任务；近7自然日补0，按首次结束本地日；历史含完成/放弃与真实金币。 |
| B08 | D设置与A首页接口 | 只读D.settings默认值；活动轮不改，下一轮采用新值；显式选择优先。StatisticsService统一供首页与统计，不把查询结果另存成第二套数字。 |

你主责的PRD/验收ID为：NAV-12, NAV-13, TASK-01, TASK-02, TASK-03, TASK-04, TASK-05, TASK-06, TASK-07, TASK-08, TASK-09, TASK-10, TASK-11, TASK-12, FOCUS-01, FOCUS-02, FOCUS-03, FOCUS-04, FOCUS-05, FOCUS-06, FOCUS-07, FOCUS-08, FOCUS-09, FOCUS-10, FOCUS-11, STAT-01, STAT-02, STAT-03, STAT-04, STAT-05, STAT-06, STAT-07, AC-03, AC-04, AC-05, AC-06, AC-07, AC-08, AC-09, AC-10, AC-11, AC-13, AC-14, AC-15, AC-17, AC-18, AC-25, AC-26。全量映射在08目录PRD覆盖与责任矩阵.csv；协助他人不表示另造同一接口。

## 2 我要从哪里开始

1. P0先读schema、数据类型和端口，拿A同一基线；不先花几天画统计图。
2. 日1优先实现Repository建表、任务持久化和C依赖的两个原子端口，并提供可用性测试；C可先依照端口替身开发，不等待B全部UI。
3. P1完成任务CRUD、筛选、共用编辑器与真实重启持久化；尽早合并给A首页使用。
4. P1实现FocusEngine真实计时、会话状态持久化与completionPersisted；与C/A闭合一次真实奖励，不自行写发币逻辑。
5. P2实现StatisticsService、统计页、今日会话列表和首页共享查询；复用A图表组件。
6. 日4补齐全部失败/冲突/恢复分支，日5完成真实25分钟与重启证据。B任务多，按上述依赖优先，全部仍须日5交付。

## 3 给AI哪些文件

| 精确仓库路径 | 交给AI的用途 |
|---|---|
| `01_全员共同遵守的规则/ENVIRONMENT_LOCK.md` | 确认同一真实Kit；未填齐先列技术阻塞 |
| `FocusGarden/docs/SOURCE_PRD.txt` | 最新有效功能与分工，搜索仅标题 |
| `FocusGarden/team/SHARED_AI_PROMPT.md` | 全员统一开发规则与前5天全部功能目标 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/DATA_MODELS.md` | 唯一类型/枚举/ID/时间和错误定义 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/API_CONTRACT.md` | 精确C++签名及谁调用谁 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/SIGNAL_SLOT_MATRIX.md` | 唯一signals/slots与触发次序 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/UI_BINDING_CONTRACT.md` | 页面构造、公共控件、模态接入 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/FILE_OWNERSHIP.json` | 唯一文件修改权限与日1移交 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/PROJECT_STRUCTURE.md` | 唯一工程位置与外部资源引用 |
| `FocusGarden/DesignTokens.json` | 颜色/尺寸/字体Tokens |
| `FocusGarden/UI_COMPONENTS.md` | 共享组件和状态，不各写一套UI |
| `FocusGarden/main.qss` | 唯一Qt样式，按动态属性使用 |
| `FocusGarden/resources.qrc` | 唯一资源表，保持相对路径与资源前缀 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/API_REGISTRY.json` | 与准确API逐项核对，不能另造签名 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/CONTRACT_LOCK.json` | 核对共同冻结版本与文件hash |
| `FocusGarden/Qt_UI_Implementation_Blueprint/PENDING_AND_DECISIONS.md` | 确认已定产品规则和真实技术阻塞 |
| `FocusGarden/UI_完整开发规范.docx` | 七页及弹窗完整视觉规范，保留V2.1 |
| `FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html` | 完整可点击原型；只能借鉴视觉交互，不把假数据当后端 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/DATABASE_CONTRACT.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/SCHEMA_V1.sql` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/tasks.json` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/focus.json` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/statistics.json` | 本模块重点读取 |
| `FocusGarden/七个页面与弹窗的组件参数表.xlsx` | 本模块重点读取 |

A发布基线后，再读取以下真实工程文件：

- `Qt_UI_Starter/CMakeLists.txt`
- `Qt_UI_Starter/src/contracts/DomainTypes.h`
- `Qt_UI_Starter/src/contracts/ServicePorts.h`
- `Qt_UI_Starter/src/services/AppServices.h`
- `Qt_UI_Starter/src/ui/Components.h`
- `Qt_UI_Starter/src/AppComposition.h`
- `Qt_UI_Starter/src/pages/TaskPage.h`
- `Qt_UI_Starter/src/pages/FocusPage.h`
- `Qt_UI_Starter/src/pages/StatisticsPage.h`
- `Qt_UI_Starter/src/services/TaskManager.h`
- `Qt_UI_Starter/src/services/FocusEngine.h`
- `Qt_UI_Starter/src/services/StatisticsService.h`
- `Qt_UI_Starter/src/services/Repository.h`
- `Qt_UI_Starter/src/ui/TaskEditDialog.h`
- `Qt_UI_Starter/src/storage/Migrations.h`

日1之前这些Qt文件尚未生成；成员拿到基线后若文件缺失，报A修复公共壳，不创建第二工程。输入清单机器版见同目录AI_INPUT_FILES.json。

## 4 文件修改权限

- `Qt_UI_Starter/src/pages/TaskPage.h`
- `Qt_UI_Starter/src/pages/TaskPage.cpp`
- `Qt_UI_Starter/src/pages/FocusPage.h`
- `Qt_UI_Starter/src/pages/FocusPage.cpp`
- `Qt_UI_Starter/src/pages/StatisticsPage.h`
- `Qt_UI_Starter/src/pages/StatisticsPage.cpp`
- `Qt_UI_Starter/src/services/TaskManager.h`
- `Qt_UI_Starter/src/services/TaskManager.cpp`
- `Qt_UI_Starter/src/services/FocusEngine.h`
- `Qt_UI_Starter/src/services/FocusEngine.cpp`
- `Qt_UI_Starter/src/services/StatisticsService.h`
- `Qt_UI_Starter/src/services/StatisticsService.cpp`
- `Qt_UI_Starter/src/services/Repository.h`
- `Qt_UI_Starter/src/services/Repository.cpp`
- `Qt_UI_Starter/src/ui/TaskEditDialog.h`
- `Qt_UI_Starter/src/ui/TaskEditDialog.cpp`
- `Qt_UI_Starter/forms/TaskEditDialog.ui`
- `Qt_UI_Starter/src/storage/Migrations.h`
- `Qt_UI_Starter/src/storage/Migrations.cpp`
- `Qt_UI_Starter/tests/b/`

禁止修改CMakeLists.txt、AppComposition、MainWindow、src/contracts、AppServices、FocusGarden公共QSS/Token/qrc及他人实现。新增构建登记/接口变化交A做最小修改；可改本人类私有实现和测试，但公共override签名不变。

## 5 精确接口与数据

所有数据结构统一引用DATA_MODELS：Task/TaskDraft/TaskFilter、FocusConfig/FocusSnapshot/FocusSession、Result/OperationResult、Preferences、PetSnapshot、统计/媒体快照与receipt。ID和时间口径不得另起定义。以下摘录由同一API注册表生成，遇版本差异以冻结API_CONTRACT为准：


### IRepository

| 必须实现的准确签名 | 调用与语义 |
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

继承使用的信号（不得在具体类重复声明）：



### ITaskService

| 必须实现的准确签名 | 调用与语义 |
|---|---|
| `Result<QVector<Task>> listTasks(const TaskFilter& filter) const;` | A/B页面经AppServices调用 |
| `Result<std::optional<Task>> task(TaskId id) const;` | 共用编辑弹窗与详情 |
| `Result<Task> saveTask(const TaskDraft& draft, std::optional<TaskId> id);` | 空id新增；有id编辑；共同校验 |
| `OperationResult setTaskDone(TaskId id, bool completed);` | 完成恢复，不发币 |
| `OperationResult softDeleteTask(TaskId id);` | 确认后软删 |

继承使用的信号（不得在具体类重复声明）：

- `void tasksChanged();`
- `void errorOccurred(fg::AppError error);`

### IStatisticsService

| 必须实现的准确签名 | 调用与语义 |
|---|---|
| `Result<StatisticsSnapshot> statistics() const;` | A/B页面调用，以当前本地日为anchor |
| `Result<QVector<Task>> todayTasks() const;` | A今日最多3项 |

继承使用的信号（不得在具体类重复声明）：



### IFocusEngine

| 必须实现的准确签名 | 调用与语义 |
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

继承使用的信号（不得在具体类重复声明）：

- `void focusChanged(fg::FocusSnapshot snapshot);`
- `void completionPersisted(fg::SessionId sessionId);`
- `void restFinished();`
- `void errorOccurred(fg::AppError error);`


页面signals/slots逐字见SIGNAL_SLOT_MATRIX；UI构造/槽逐字见UI_BINDING_CONTRACT。A的门面统一转发，B/C/D具体类只实现本人端口。写入失败返回中文错误，不能伪报成功或关闭需重试的窗口。

## 6 可直接复制的完整开发指令

下面这份内容可整段交给Codex；同目录给Codex的完整指令.md是方便复制的纯文本版。

# 给Codex的完整指令 角色B

你现在是Focus Garden四人小组的B成员，负责任务 专注 统计 SQLite数据库。这是在队长已经建立的同一Qt工程内的一次完整任务，不是只做静态UI。第5天前必须交付本文全部真实功能，数据持久化和异常分支一并实现。第6天后仅允许测试、修复、优化、部署和报告答辩，不把任何必做业务留到后续再做。

先读取本角色AI输入文件清单中的每一个当前必读路径，并核对ENVIRONMENT_LOCK与FG-API-1.0。下面列出精确必读路径：

- 01_全员共同遵守的规则/ENVIRONMENT_LOCK.md
- FocusGarden/docs/SOURCE_PRD.txt
- FocusGarden/team/SHARED_AI_PROMPT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/DATA_MODELS.md
- FocusGarden/Qt_UI_Implementation_Blueprint/API_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/SIGNAL_SLOT_MATRIX.md
- FocusGarden/Qt_UI_Implementation_Blueprint/UI_BINDING_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/FILE_OWNERSHIP.json
- FocusGarden/Qt_UI_Implementation_Blueprint/PROJECT_STRUCTURE.md
- FocusGarden/DesignTokens.json
- FocusGarden/UI_COMPONENTS.md
- FocusGarden/main.qss
- FocusGarden/resources.qrc
- FocusGarden/Qt_UI_Implementation_Blueprint/API_REGISTRY.json
- FocusGarden/Qt_UI_Implementation_Blueprint/CONTRACT_LOCK.json
- FocusGarden/Qt_UI_Implementation_Blueprint/PENDING_AND_DECISIONS.md
- FocusGarden/UI_完整开发规范.docx
- FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html
- FocusGarden/Qt_UI_Implementation_Blueprint/DATABASE_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/SCHEMA_V1.sql
- FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md
- FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/tasks.json
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/focus.json
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/statistics.json
- FocusGarden/七个页面与弹窗的组件参数表.xlsx

在A发布后额外读取：

- Qt_UI_Starter/CMakeLists.txt
- Qt_UI_Starter/src/contracts/DomainTypes.h
- Qt_UI_Starter/src/contracts/ServicePorts.h
- Qt_UI_Starter/src/services/AppServices.h
- Qt_UI_Starter/src/ui/Components.h
- Qt_UI_Starter/src/AppComposition.h
- Qt_UI_Starter/src/pages/TaskPage.h
- Qt_UI_Starter/src/pages/FocusPage.h
- Qt_UI_Starter/src/pages/StatisticsPage.h
- Qt_UI_Starter/src/services/TaskManager.h
- Qt_UI_Starter/src/services/FocusEngine.h
- Qt_UI_Starter/src/services/StatisticsService.h
- Qt_UI_Starter/src/services/Repository.h
- Qt_UI_Starter/src/ui/TaskEditDialog.h
- Qt_UI_Starter/src/storage/Migrations.h

如果A基线、实际公共声明或Kit未准备好，明确报告缺少的路径/值；禁止另建第二个Qt项目。A成员执行本段时按队长第1天手册先创建唯一基线，随后继续完成完整A任务，不能把支架发布当成A全部交付。


唯一结构契约版本FG-API-1.0。最新分工：A主窗口/总览/公共UI/整合；B任务/专注/统计/SQLite；C宠物/金币/喂食升级；D音乐/设置/测试部署。SettingsPage和SettingsService由D实现，A只接入导航与门面。搜索仅标题，已由队长确认。

前5天必须实现PRD全部必做功能和真实持久化；首页读取真实服务，B关闭重开保留任务/专注，C只从真实完成记录结算，D真播本地音频并持久化设置。第6天以后仅测试、修复、UI/性能优化、Windows部署及报告答辩。Mock只允许日1启动支架和单元测试，不能充当日5成果。

只在同一个仓库中的Qt_UI_Starter工程增量开发；先读ENVIRONMENT_LOCK，Qt/编译器/Kit不明先列阻塞；先检查A的基线提交与contracts，不另起完整项目。A第1天先建立并发布基础工程、共享声明和可编译占位；B/C/D在原文件接管实现。当前开工资料包不含Qt工程，不能声称本包已编译。

接口签名只以API_CONTRACT、DATA_MODELS、SIGNAL_SLOT_MATRIX、UI_BINDING_CONTRACT为准；API_REGISTRY是机器核对副本。A唯一生成维护DomainTypes.h和ServicePorts.h；其余人实现自己的类，不能另定义Task/Session/Result或改公共签名绕过错误。需变更时提交最小接口变更说明交A审核，再全员拉取同一版本。

UI保持V2.1黄绿奶油白、七页布局、ID/ObjectName、公共QSS和:/fg/UI_ASSETS路径。Qt Layout适配三尺寸；禁止WebEngine替代Widgets、全量setGeometry、复制页面自己的QSS或资源。仓库FocusGarden/保留唯一资源，CMake引用其qrc，不复制图片到各角色目录。

Repository只有B创建一个Live连接和执行SQL；C通过冻结原子端口结算/喂食，不另开库；D唯一写QSettings；所有页通过A门面请求。切页不重建页面/服务/计时器/播放器，刷新不写SQL。错误返回按契约处理，保存失败不关窗、不发币、不伪报成功。

不得新增账号/通知中心/商店/抚摸/背包/Lv4/在线音乐/环境音/主题换肤/导出等范围外功能。任务完成不发金币；从任务进入专注只预选，由用户手动开始。

每个可运行改动持续提交，小PR尽早合并，不等第5天拼接。提交必须含实际代码、真实构建/测试日志、运行截图、验收记录和未完成问题。没有环境或没运行的检查明确写未执行；不能把加速测试当真实25分钟耐久测试，不能只交截图。完成定义以第5天全功能验收表为准。


你的完整功能范围：

- 任务和共用编辑器：新增/编辑/完成/恢复/软删除；标题trim后1～100码点，备注<=500，学习/工作/生活，高/中/低，截止可空可过去。新增编辑同TaskEditDialog；dirty取消/X/Esc二层确认保留输入。
- 查询筛选与详情：仅标题实时搜索，分类/优先级/状态AND叠加；截止升序、空截止最后、相同截止创建时间倒序。点击整行只读详情；跨页保留筛选和选中。已完成可编辑删除恢复但不新开专注。
- 唯一SQLite仓库：实现FG-API全部IRepository端口和schema迁移，唯一连接；任务/会话读写及供C调用的奖励/喂食原子方法。重启保留数据，正式库不注入演示样例。
- 真实计时：25/50，Custom15～90步长5，Rest5/10；手动开始、暂停、继续、确认放弃、自然完成；QElapsedTimer活动毫秒，QTimer只刷新；离页和最小化继续。
- 会话与奖励边界：开始保存唯一SessionId及标题快照；保存Completed成功才发completionPersisted，A协调C；保存失败不发币，结算失败保留Completed/Pending，重试保留原ID/结束日。
- 任务冲突与异常恢复：活动任务修改/完成/删除不打断现轮，历史用快照；另一任务专注入口保留原会话；放弃保存失败保留计时；关闭确认期间自然完成不可覆写；异常重启遗留Running/Paused转Abandoned零币。
- 统计真实口径：今日/累计只计Completed活动秒，未结算仍计时长；已完成任务/全量未删除完成率，0分母显示暂无任务；近7自然日补0，按首次结束本地日；历史含完成/放弃与真实金币。
- D设置与A首页接口：只读D.settings默认值；活动轮不改，下一轮采用新值；显式选择优先。StatisticsService统一供首页与统计，不把查询结果另存成第二套数字。

允许编辑的计划文件或目录如下（A的初始占位在发布基线后按所有权移交）：

- `Qt_UI_Starter/src/pages/TaskPage.h`
- `Qt_UI_Starter/src/pages/TaskPage.cpp`
- `Qt_UI_Starter/src/pages/FocusPage.h`
- `Qt_UI_Starter/src/pages/FocusPage.cpp`
- `Qt_UI_Starter/src/pages/StatisticsPage.h`
- `Qt_UI_Starter/src/pages/StatisticsPage.cpp`
- `Qt_UI_Starter/src/services/TaskManager.h`
- `Qt_UI_Starter/src/services/TaskManager.cpp`
- `Qt_UI_Starter/src/services/FocusEngine.h`
- `Qt_UI_Starter/src/services/FocusEngine.cpp`
- `Qt_UI_Starter/src/services/StatisticsService.h`
- `Qt_UI_Starter/src/services/StatisticsService.cpp`
- `Qt_UI_Starter/src/services/Repository.h`
- `Qt_UI_Starter/src/services/Repository.cpp`
- `Qt_UI_Starter/src/ui/TaskEditDialog.h`
- `Qt_UI_Starter/src/ui/TaskEditDialog.cpp`
- `Qt_UI_Starter/forms/TaskEditDialog.ui`
- `Qt_UI_Starter/src/storage/Migrations.h`
- `Qt_UI_Starter/src/storage/Migrations.cpp`
- `Qt_UI_Starter/tests/b/`

修改限制：禁止修改CMakeLists.txt、AppComposition、MainWindow、src/contracts、AppServices、FocusGarden公共QSS/Token/qrc及他人实现。新增构建登记/接口变化交A做最小修改；可改本人类私有实现和测试，但公共override签名不变。

按这个顺序实际开发，不只给计划：

1. P0先读schema、数据类型和端口，拿A同一基线；不先花几天画统计图。
2. 日1优先实现Repository建表、任务持久化和C依赖的两个原子端口，并提供可用性测试；C可先依照端口替身开发，不等待B全部UI。
3. P1完成任务CRUD、筛选、共用编辑器与真实重启持久化；尽早合并给A首页使用。
4. P1实现FocusEngine真实计时、会话状态持久化与completionPersisted；与C/A闭合一次真实奖励，不自行写发币逻辑。
5. P2实现StatisticsService、统计页、今日会话列表和首页共享查询；复用A图表组件。
6. 日4补齐全部失败/冲突/恢复分支，日5完成真实25分钟与重启证据。B任务多，按上述依赖优先，全部仍须日5交付。

准确实现IRepository, ITaskService, IStatisticsService, IFocusEngine在API_CONTRACT中的全部方法，按DATA_MODELS生成/使用字段与枚举，按SIGNAL_SLOT_MATRIX触发信号。你的页面构造、公共组件与模态遵守UI_BINDING_CONTRACT。B/C/D具体服务继承端口的signals，不重复声明；A具体AppServices和CompletionCoordinator按契约在本类声明signals一次，不复制门面。Repository原子事务与唯一索引遵守DATABASE_CONTRACT。

完成前逐项操作并记录：

- B-T01 空库新增任务、重启、编辑并再次重启 → 标题/备注/截止/完成状态保留，createdAt不因编辑改变。
- B-T02 提交空白/101码点标题与501码点备注，模拟写入失败 → 校验不入库；失败不关窗；旧值保持。
- B-T03 表单改动后点取消/X/Esc，再选继续编辑 → 二层确认出现，继续编辑的输入完全保留。
- B-T04 仅在备注放关键词，标题无该词；再组合三筛选 → 不因备注命中；排序正确，跨页筛选仍在。
- B-T05 完成/恢复/软删带历史任务，检查金币与详情 → 不发币；已完成专注禁用；软删不删历史快照，详情可清空。
- B-T06 手动开始、等待、暂停30秒、继续、切六页与最小化 → 暂停不扣时间；同SessionId，后台继续且不多建计时器。
- B-T07 活动时改/删/完成关联任务，再点另一个任务专注 → 原会话和标题快照保留；新任务不覆盖计时。
- B-T08 自然完成一次真实25分钟，不只用加速按钮 → 保存1500有效秒，C仅加20，首页/统计/宠物更新。
- B-T09 注入completeSession失败再重试，跨午夜场景测试 → 失败零币；重试仍原SessionId与首次结束日，不多建记录。
- B-T10 C结算失败再重试；休息自然完成；放弃取消/确认 → Pending不抹掉完成时长；休息/放弃无金币和有效专注。
- B-T11 活动时异常终止再启动，正常关闭放弃写入失败 → 遗留活动标Abandoned不补奖；失败不能伪报退出已保存。
- B-T12 D改默认50/休息10；当前轮继续，下一轮手动开始 → 当前计划不变，下一轮读新值；关闭提示音交D/A验证。
- B-T13 空统计、跨午夜完成、任务数为0，核对7天和首页 → 0值无除零；结束日归档；7项补0；全部来源同统计接口。
- B-T14 C原子方法故障注入、唯一索引与回滚测试 → 同session重复结算只一收入；喂食流水和EXP/心情一起提交或一起回滚。

必须输出：

- 源代码：在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。
- 业务与接口：列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。
- 构建与测试：真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。
- 运行证据：本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。
- 验收记录：逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。
- 已知问题：重现步骤、影响、原因或待查、负责人、修复提交/复测状态。
- Git交付：推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。

额外提交schema迁移说明、Repository原子方法测试、数据口径核对表和真实25分钟/异常重启记录。

把证据放到08_测试验收与交付清单/evidence/B/，不要把正式用户数据库、账号凭据或无权分发的歌曲加入仓库。提交说明写需求ID、实现文件、命令/退出码、截图路径、未完成项和公共接口是否改变。发现接口冲突先停在冲突边界并提交最小变更说明交A审核；其余不依赖部分可继续，不用兼容包装另造接口。

本任务是一次性全范围布置，各日程是进度检查点，不需要队长每天重新发任务。没有真实代码、构建/运行结果和功能证据不能写已完成；测试环境不支持的检查列未执行，交组长判断，不能用模拟页冒充全功能软件。


## 7 AI完成后必须交什么

# B 的代码与测试交付清单

| 交付项 | 必须包含什么 |
|---|---|
| 源代码 | 在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。 |
| 业务与接口 | 列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。 |
| 构建与测试 | 真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。 |
| 运行证据 | 本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。 |
| 验收记录 | 逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。 |
| 已知问题 | 重现步骤、影响、原因或待查、负责人、修复提交/复测状态。 |
| Git交付 | 推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。 |

额外提交schema迁移说明、Repository原子方法测试、数据口径核对表和真实25分钟/异常重启记录。

| 编号 | 实际操作 | 完成标准 |
|---|---|---|
| B-T01 | 空库新增任务、重启、编辑并再次重启 | 标题/备注/截止/完成状态保留，createdAt不因编辑改变。 |
| B-T02 | 提交空白/101码点标题与501码点备注，模拟写入失败 | 校验不入库；失败不关窗；旧值保持。 |
| B-T03 | 表单改动后点取消/X/Esc，再选继续编辑 | 二层确认出现，继续编辑的输入完全保留。 |
| B-T04 | 仅在备注放关键词，标题无该词；再组合三筛选 | 不因备注命中；排序正确，跨页筛选仍在。 |
| B-T05 | 完成/恢复/软删带历史任务，检查金币与详情 | 不发币；已完成专注禁用；软删不删历史快照，详情可清空。 |
| B-T06 | 手动开始、等待、暂停30秒、继续、切六页与最小化 | 暂停不扣时间；同SessionId，后台继续且不多建计时器。 |
| B-T07 | 活动时改/删/完成关联任务，再点另一个任务专注 | 原会话和标题快照保留；新任务不覆盖计时。 |
| B-T08 | 自然完成一次真实25分钟，不只用加速按钮 | 保存1500有效秒，C仅加20，首页/统计/宠物更新。 |
| B-T09 | 注入completeSession失败再重试，跨午夜场景测试 | 失败零币；重试仍原SessionId与首次结束日，不多建记录。 |
| B-T10 | C结算失败再重试；休息自然完成；放弃取消/确认 | Pending不抹掉完成时长；休息/放弃无金币和有效专注。 |
| B-T11 | 活动时异常终止再启动，正常关闭放弃写入失败 | 遗留活动标Abandoned不补奖；失败不能伪报退出已保存。 |
| B-T12 | D改默认50/休息10；当前轮继续，下一轮手动开始 | 当前计划不变，下一轮读新值；关闭提示音交D/A验证。 |
| B-T13 | 空统计、跨午夜完成、任务数为0，核对7天和首页 | 0值无除零；结束日归档；7项补0；全部来源同统计接口。 |
| B-T14 | C原子方法故障注入、唯一索引与回滚测试 | 同session重复结算只一收入；喂食流水和EXP/心情一起提交或一起回滚。 |


## 8 五天内的检查点

| 日程 | 截止检查点 |
|---|---|
| 日1 | P0仓库DDL/迁移、任务存储和C原子端口优先；发布端口可用说明，任务UI可继续并行。 |
| 日2 | 任务CRUD/筛选/编辑器全部真实持久化；Focus状态写入与D默认值接入。 |
| 日3 | 真实FocusEngine与C/A完成链路，暂停/继续/放弃/自然完成。 |
| 日4 | 统计/历史/首页查询和全部异常、快照、恢复、跨日分支。 |
| 日5 | 全部B功能、真实25分钟、重启和失败用例证据；不得只交UI或内存仓库。 |

以上检查点不缩减第5天全功能目标。失败/未完成必须如实列明，不改验收口径，不安排第二轮必做功能开发。第6～28天按全组计划只做运行验收、Bug修复、视觉/性能稳定性、Windows部署、实验报告与答辩演示。

## 9 持续协作

从main同一基线建立本角色feature分支，完成一个能编译的小改动就提交、推送、开PR给A。日1起持续合并，禁止第5天才首次拼接。改共享接口先提变更说明，A核对FG-API版本、B复核公共声明后再合并，全员Fetch/Pull更新；不要各自临时改返回类型或加同名模型。
