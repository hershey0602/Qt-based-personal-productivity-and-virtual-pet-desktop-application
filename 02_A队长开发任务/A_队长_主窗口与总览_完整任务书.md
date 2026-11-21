# A_队长_主窗口与总览_完整任务书

你负责队长 主窗口 总览 公共UI与整合。这是从开工到最后交付的一次性完整任务。前5天实现全部分配功能与真实持久化，后续只测试修复优化部署答辩；Mock仅日1支架/测试替身。SettingsPage与SettingsService由D实现，A只装配与统一UI。功能以有效PRD、视觉以UI V2.1、接口以FG-API-1.0为准。

## 1 我负责什么

| 编号 | 范围 | 必做内容 |
|---|---|---|
| A01 | 主窗口和七页导航 | 唯一QMainWindow；一次创建七页，侧栏切页保持任务筛选、专注、音乐；原生窗口关闭确认；固定Header品牌/本地运行，无头像通知。 |
| A02 | Dashboard真实数据 | 日期/今日最多3项任务/今日有效专注/七日图/全量任务完成比例/金币/布丁等级心情经验；全部从B/C经AppServices获取。空库为0和空态，禁止写死演示值。 |
| A03 | 总览入口 | 新增打开B的共用任务编辑器；勾选完成或恢复不发币；查看任务/统计/宠物/音乐仅导航；25/50快捷只预选，手动开始由Focus页。 |
| A04 | 公共组件与弹窗 | Sidebar、TopHeader、BaseCard、MetricCard、按钮/输入/标签、TimerRing、WeekBarChart、StateView、Toast、ConfirmDialog、ResultDialog；B负责TaskEditDialog业务，A统一外观。 |
| A05 | 公共模型和接口 | 按FG-API-1.0一次生成DomainTypes.h和ServicePorts.h。AppServices只转发，CompletionCoordinator串联B保存和C结算；A不另实现任务/奖励/设置业务。 |
| A06 | 应用级装配与刷新 | AppComposition只持有一组服务；成功信号排队刷新；自然完成在其他页也到账并提示；设置归D，A只接入SettingsPage、注入接口与信号。 |
| A07 | 资源与自适应 | 从../FocusGarden/resources.qrc编入唯一工程，45引用可加载；保留黄绿奶油白和组件ID，三尺寸布局和纵向滚动。 |
| A08 | 启动恢复及退出协调 | 装配顺序含B恢复遗留会话、枚举Pending交C补偿；活动关闭确认成功持久化放弃后退出；失败留窗口；Pending已保存可正常退出后恢复。 |
| A09 | 整合和最终资料 | 从日1持续小PR；日5整合全部真实功能；汇总代码/日志/验收/已知问题；日6后仅测试修复优化部署和报告答辩。 |

你主责的PRD/验收ID为：NAV-01, NAV-02, NAV-03, NAV-04, NAV-05, NAV-06, NAV-07, NAV-08, NAV-09, NAV-10, NAV-11, NAV-14, NAV-18, NAV-19, DASH-01, DASH-02, DASH-03, DASH-04, DASH-05, DASH-06, DASH-07, DASH-08, DASH-09, DASH-10, AC-01, AC-02, AC-12, AC-19, AC-28, AC-31, AC-32, AC-33。全量映射在08目录PRD覆盖与责任矩阵.csv；协助他人不表示另造同一接口。

## 2 我要从哪里开始

1. 先完成环境锁定，阅读第1天手册与冻结接口；在feature/a-shell建立唯一工程，不能为每人各建工程。
2. 日1最早交公共模型/接口、七页入口、QSS/qrc、MockPorts和具体类占位，编译运行后发布main基线及SHA。
3. 接着完成Dashboard与公共组件、共用确认/结果框；注册B的编辑弹窗入口，页面实例保持不变。
4. B/C/D交服务后，在AppComposition将Mock逐个换为真实类，维护唯一门面转发及Coordinator；不修改成员内部逻辑绕过接口。
5. 完成首页真实快照和跨页更新；联合B/C验证保存→结算唯一流水→总览/宠物/统计同步。
6. 日5在Live空库跑全链路、重启、三尺寸和真实自然完成；缺项标未完成，不用Mock替代。

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
| `FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html` | 完整可点击原型；只能借鉴视觉交互，不把假数据当后端 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/dashboard.json` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/COMPONENT_CONTRACTS.json` | 本模块重点读取 |
| `FocusGarden/UI_完整开发规范.docx` | 本模块重点读取 |
| `FocusGarden/七个页面与弹窗的组件参数表.xlsx` | 本模块重点读取 |
| `00_队长从这里开始/队长第1天操作手册.md` | 本模块重点读取 |

A发布基线后，再读取以下真实工程文件：

- `Qt_UI_Starter/CMakeLists.txt`
- `Qt_UI_Starter/src/contracts/DomainTypes.h`
- `Qt_UI_Starter/src/contracts/ServicePorts.h`
- `Qt_UI_Starter/src/services/AppServices.h`
- `Qt_UI_Starter/src/ui/Components.h`
- `Qt_UI_Starter/src/AppComposition.h`
- `Qt_UI_Starter/src/MainWindow.h`
- `Qt_UI_Starter/src/services/CompletionCoordinator.h`
- `Qt_UI_Starter/src/pages/DashboardPage.h`
- `Qt_UI_Starter/src/ui/ConfirmDialog.h`
- `Qt_UI_Starter/src/ui/ResultDialog.h`
- `Qt_UI_Starter/src/mocks/MockPorts.h`

日1之前这些Qt文件尚未生成；成员拿到基线后若文件缺失，报A修复公共壳，不创建第二工程。输入清单机器版见同目录AI_INPUT_FILES.json。

## 4 文件修改权限

- `Qt_UI_Starter/CMakeLists.txt`
- `Qt_UI_Starter/src/main.cpp`
- `Qt_UI_Starter/src/MainWindow.h`
- `Qt_UI_Starter/src/MainWindow.cpp`
- `Qt_UI_Starter/src/AppComposition.h`
- `Qt_UI_Starter/src/AppComposition.cpp`
- `Qt_UI_Starter/src/contracts/DomainTypes.h`
- `Qt_UI_Starter/src/contracts/ServicePorts.h`
- `Qt_UI_Starter/src/services/AppServices.h`
- `Qt_UI_Starter/src/services/AppServices.cpp`
- `Qt_UI_Starter/src/services/CompletionCoordinator.h`
- `Qt_UI_Starter/src/services/CompletionCoordinator.cpp`
- `Qt_UI_Starter/src/pages/DashboardPage.h`
- `Qt_UI_Starter/src/pages/DashboardPage.cpp`
- `Qt_UI_Starter/src/ui/Components.h`
- `Qt_UI_Starter/src/ui/Components.cpp`
- `Qt_UI_Starter/src/ui/ConfirmDialog.h`
- `Qt_UI_Starter/src/ui/ConfirmDialog.cpp`
- `Qt_UI_Starter/src/ui/ResultDialog.h`
- `Qt_UI_Starter/src/ui/ResultDialog.cpp`
- `Qt_UI_Starter/src/mocks/MockPorts.h`
- `Qt_UI_Starter/src/mocks/MockPorts.cpp`
- `Qt_UI_Starter/tools/`
- `Qt_UI_Starter/tests/test_shell.cpp`
- `FocusGarden/DesignTokens.json`
- `FocusGarden/main.qss`
- `FocusGarden/resources.qrc`
- `FocusGarden/UI_COMPONENTS.md`
- `FocusGarden/Qt_UI_Implementation_Blueprint/`

仅你负责公共文件合并；公共签名变动先记录变更，B复核后再让全员更新。日1可生成成员占位，发布后不覆盖成员实现。

## 5 精确接口与数据

所有数据结构统一引用DATA_MODELS：Task/TaskDraft/TaskFilter、FocusConfig/FocusSnapshot/FocusSession、Result/OperationResult、Preferences、PetSnapshot、统计/媒体快照与receipt。ID和时间口径不得另起定义。以下摘录由同一API注册表生成，遇版本差异以冻结API_CONTRACT为准：


### AppServices

| 必须实现的准确签名 | 调用与语义 |
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

本A具体类需声明的信号（只在本类声明一次）：

- `void dataChanged();`
- `void focusChanged(fg::FocusSnapshot snapshot);`
- `void preferencesChanged(fg::Preferences preferences);`
- `void musicChanged(fg::MusicSnapshot snapshot);`
- `void focusCompleted(fg::CompletionResult result);`
- `void petLeveled(fg::FeedReceipt receipt);`
- `void notice(QString message, fg::NoticeLevel level);`

### CompletionCoordinator

| 必须实现的准确签名 | 调用与语义 |
|---|---|
| `void settle(fg::SessionId sessionId);` | Qt slot；接B completionPersisted，调用C一次结算 |
| `OperationResult retry(const SessionId& sessionId);` | SaveFailed先B重试保存；Pending仅C重试结算 |
| `OperationResult retryPending();` | 启动恢复后/进入Focus枚举已保存未结算记录；失败不循环轰炸提示 |

本A具体类需声明的信号（只在本类声明一次）：

- `void settlementCompleted(fg::SettlementReceipt receipt);`
- `void settlementFailed(fg::SessionId sessionId, fg::AppError error);`


页面signals/slots逐字见SIGNAL_SLOT_MATRIX；UI构造/槽逐字见UI_BINDING_CONTRACT。A的门面统一转发，B/C/D具体类只实现本人端口。写入失败返回中文错误，不能伪报成功或关闭需重试的窗口。

## 6 可直接复制的完整开发指令

下面这份内容可整段交给Codex；同目录给Codex的完整指令.md是方便复制的纯文本版。

# 给Codex的完整指令 角色A

你现在是Focus Garden四人小组的A成员，负责队长 主窗口 总览 公共UI与整合。这是在队长已经建立的同一Qt工程内的一次完整任务，不是只做静态UI。第5天前必须交付本文全部真实功能，数据持久化和异常分支一并实现。第6天后仅允许测试、修复、优化、部署和报告答辩，不把任何必做业务留到后续再做。

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
- FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html
- FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md
- FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/dashboard.json
- FocusGarden/Qt_UI_Implementation_Blueprint/COMPONENT_CONTRACTS.json
- FocusGarden/UI_完整开发规范.docx
- FocusGarden/七个页面与弹窗的组件参数表.xlsx
- 00_队长从这里开始/队长第1天操作手册.md

在A发布后额外读取：

- Qt_UI_Starter/CMakeLists.txt
- Qt_UI_Starter/src/contracts/DomainTypes.h
- Qt_UI_Starter/src/contracts/ServicePorts.h
- Qt_UI_Starter/src/services/AppServices.h
- Qt_UI_Starter/src/ui/Components.h
- Qt_UI_Starter/src/AppComposition.h
- Qt_UI_Starter/src/MainWindow.h
- Qt_UI_Starter/src/services/CompletionCoordinator.h
- Qt_UI_Starter/src/pages/DashboardPage.h
- Qt_UI_Starter/src/ui/ConfirmDialog.h
- Qt_UI_Starter/src/ui/ResultDialog.h
- Qt_UI_Starter/src/mocks/MockPorts.h

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

- 主窗口和七页导航：唯一QMainWindow；一次创建七页，侧栏切页保持任务筛选、专注、音乐；原生窗口关闭确认；固定Header品牌/本地运行，无头像通知。
- Dashboard真实数据：日期/今日最多3项任务/今日有效专注/七日图/全量任务完成比例/金币/布丁等级心情经验；全部从B/C经AppServices获取。空库为0和空态，禁止写死演示值。
- 总览入口：新增打开B的共用任务编辑器；勾选完成或恢复不发币；查看任务/统计/宠物/音乐仅导航；25/50快捷只预选，手动开始由Focus页。
- 公共组件与弹窗：Sidebar、TopHeader、BaseCard、MetricCard、按钮/输入/标签、TimerRing、WeekBarChart、StateView、Toast、ConfirmDialog、ResultDialog；B负责TaskEditDialog业务，A统一外观。
- 公共模型和接口：按FG-API-1.0一次生成DomainTypes.h和ServicePorts.h。AppServices只转发，CompletionCoordinator串联B保存和C结算；A不另实现任务/奖励/设置业务。
- 应用级装配与刷新：AppComposition只持有一组服务；成功信号排队刷新；自然完成在其他页也到账并提示；设置归D，A只接入SettingsPage、注入接口与信号。
- 资源与自适应：从../FocusGarden/resources.qrc编入唯一工程，45引用可加载；保留黄绿奶油白和组件ID，三尺寸布局和纵向滚动。
- 启动恢复及退出协调：装配顺序含B恢复遗留会话、枚举Pending交C补偿；活动关闭确认成功持久化放弃后退出；失败留窗口；Pending已保存可正常退出后恢复。
- 整合和最终资料：从日1持续小PR；日5整合全部真实功能；汇总代码/日志/验收/已知问题；日6后仅测试修复优化部署和报告答辩。

允许编辑的计划文件或目录如下（A的初始占位在发布基线后按所有权移交）：

- `Qt_UI_Starter/CMakeLists.txt`
- `Qt_UI_Starter/src/main.cpp`
- `Qt_UI_Starter/src/MainWindow.h`
- `Qt_UI_Starter/src/MainWindow.cpp`
- `Qt_UI_Starter/src/AppComposition.h`
- `Qt_UI_Starter/src/AppComposition.cpp`
- `Qt_UI_Starter/src/contracts/DomainTypes.h`
- `Qt_UI_Starter/src/contracts/ServicePorts.h`
- `Qt_UI_Starter/src/services/AppServices.h`
- `Qt_UI_Starter/src/services/AppServices.cpp`
- `Qt_UI_Starter/src/services/CompletionCoordinator.h`
- `Qt_UI_Starter/src/services/CompletionCoordinator.cpp`
- `Qt_UI_Starter/src/pages/DashboardPage.h`
- `Qt_UI_Starter/src/pages/DashboardPage.cpp`
- `Qt_UI_Starter/src/ui/Components.h`
- `Qt_UI_Starter/src/ui/Components.cpp`
- `Qt_UI_Starter/src/ui/ConfirmDialog.h`
- `Qt_UI_Starter/src/ui/ConfirmDialog.cpp`
- `Qt_UI_Starter/src/ui/ResultDialog.h`
- `Qt_UI_Starter/src/ui/ResultDialog.cpp`
- `Qt_UI_Starter/src/mocks/MockPorts.h`
- `Qt_UI_Starter/src/mocks/MockPorts.cpp`
- `Qt_UI_Starter/tools/`
- `Qt_UI_Starter/tests/test_shell.cpp`
- `FocusGarden/DesignTokens.json`
- `FocusGarden/main.qss`
- `FocusGarden/resources.qrc`
- `FocusGarden/UI_COMPONENTS.md`
- `FocusGarden/Qt_UI_Implementation_Blueprint/`

修改限制：仅你负责公共文件合并；公共签名变动先记录变更，B复核后再让全员更新。日1可生成成员占位，发布后不覆盖成员实现。

按这个顺序实际开发，不只给计划：

1. 先完成环境锁定，阅读第1天手册与冻结接口；在feature/a-shell建立唯一工程，不能为每人各建工程。
2. 日1最早交公共模型/接口、七页入口、QSS/qrc、MockPorts和具体类占位，编译运行后发布main基线及SHA。
3. 接着完成Dashboard与公共组件、共用确认/结果框；注册B的编辑弹窗入口，页面实例保持不变。
4. B/C/D交服务后，在AppComposition将Mock逐个换为真实类，维护唯一门面转发及Coordinator；不修改成员内部逻辑绕过接口。
5. 完成首页真实快照和跨页更新；联合B/C验证保存→结算唯一流水→总览/宠物/统计同步。
6. 日5在Live空库跑全链路、重启、三尺寸和真实自然完成；缺项标未完成，不用Mock替代。

准确实现AppServices, CompletionCoordinator在API_CONTRACT中的全部方法，按DATA_MODELS生成/使用字段与枚举，按SIGNAL_SLOT_MATRIX触发信号。你的页面构造、公共组件与模态遵守UI_BINDING_CONTRACT。B/C/D具体服务继承端口的signals，不重复声明；A具体AppServices和CompletionCoordinator按契约在本类声明signals一次，不复制门面。Repository原子事务与唯一索引遵守DATABASE_CONTRACT。

完成前逐项操作并记录：

- A-T01 启动后依次点七页，再回Tasks/Focus/Music → 没有第二个MainWindow/服务；筛选和后台状态保持。
- A-T02 空库启动；首页新增今日截止任务并勾选 → 空态变真实今日任务，任务页/统计同步；金币不变。
- A-T03 从首页选25/50、去喂养和音乐 → 只导航/预选，不自动计时、扣币或播歌。
- A-T04 在Tasks页等待真实25分钟自然结束 → B保存、C结算，首页/宠物/统计实时刷新，全局只一次到账提示。
- A-T05 运行中关闭，先取消再确认；注入放弃保存失败 → 取消不中断；成功才退出；失败留窗；不发放弃金币。
- A-T06 完成后注入C结算失败，关闭重开再重试 → 完成记录仍在，余额未提前增加；恢复成功只入一次流水。
- A-T07 在三尺寸加载七页与所有公共弹窗 → 图标/PNG真实存在，无横向溢出或文字裁切，视觉对照V2.1。
- A-T08 查Live装配、应用对象和Git历史 → 日5无Mock冒充正式数据；只有一个工程；日1起有持续提交与整合记录。

必须输出：

- 源代码：在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。
- 业务与接口：列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。
- 构建与测试：真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。
- 运行证据：本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。
- 验收记录：逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。
- 已知问题：重现步骤、影响、原因或待查、负责人、修复提交/复测状态。
- Git交付：推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。

额外提交AppComposition装配说明、唯一基线提交SHA、FG-API版本记录、全组验收汇总和接口变更记录。

把证据放到08_测试验收与交付清单/evidence/A/，不要把正式用户数据库、账号凭据或无权分发的歌曲加入仓库。提交说明写需求ID、实现文件、命令/退出码、截图路径、未完成项和公共接口是否改变。发现接口冲突先停在冲突边界并提交最小变更说明交A审核；其余不依赖部分可继续，不用兼容包装另造接口。

本任务是一次性全范围布置，各日程是进度检查点，不需要队长每天重新发任务。没有真实代码、构建/运行结果和功能证据不能写已完成；测试环境不支持的检查列未执行，交组长判断，不能用模拟页冒充全功能软件。


## 7 AI完成后必须交什么

# A 的代码与测试交付清单

| 交付项 | 必须包含什么 |
|---|---|
| 源代码 | 在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。 |
| 业务与接口 | 列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。 |
| 构建与测试 | 真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。 |
| 运行证据 | 本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。 |
| 验收记录 | 逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。 |
| 已知问题 | 重现步骤、影响、原因或待查、负责人、修复提交/复测状态。 |
| Git交付 | 推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。 |

额外提交AppComposition装配说明、唯一基线提交SHA、FG-API版本记录、全组验收汇总和接口变更记录。

| 编号 | 实际操作 | 完成标准 |
|---|---|---|
| A-T01 | 启动后依次点七页，再回Tasks/Focus/Music | 没有第二个MainWindow/服务；筛选和后台状态保持。 |
| A-T02 | 空库启动；首页新增今日截止任务并勾选 | 空态变真实今日任务，任务页/统计同步；金币不变。 |
| A-T03 | 从首页选25/50、去喂养和音乐 | 只导航/预选，不自动计时、扣币或播歌。 |
| A-T04 | 在Tasks页等待真实25分钟自然结束 | B保存、C结算，首页/宠物/统计实时刷新，全局只一次到账提示。 |
| A-T05 | 运行中关闭，先取消再确认；注入放弃保存失败 | 取消不中断；成功才退出；失败留窗；不发放弃金币。 |
| A-T06 | 完成后注入C结算失败，关闭重开再重试 | 完成记录仍在，余额未提前增加；恢复成功只入一次流水。 |
| A-T07 | 在三尺寸加载七页与所有公共弹窗 | 图标/PNG真实存在，无横向溢出或文字裁切，视觉对照V2.1。 |
| A-T08 | 查Live装配、应用对象和Git历史 | 日5无Mock冒充正式数据；只有一个工程；日1起有持续提交与整合记录。 |


## 8 五天内的检查点

| 日程 | 截止检查点 |
|---|---|
| 日1 | 发布可编译公共基线；共享声明和七页导航/服务接入点齐备；首个基线SHA交B/C/D。 |
| 日2 | Dashboard/统一组件与全部公共确认结果框；接入已合并的真实Repository和SettingsService。 |
| 日3 | 闭合B完成→C结算→三页更新；真实音乐和设置接入；切页不重建。 |
| 日4 | 全部页面/弹窗与异常路径整合，清除正式数据演示硬编码。 |
| 日5 | Live全功能验收、真实数据首页、重启、自然完成及跨模块证据汇总。 |

以上检查点不缩减第5天全功能目标。失败/未完成必须如实列明，不改验收口径，不安排第二轮必做功能开发。第6～28天按全组计划只做运行验收、Bug修复、视觉/性能稳定性、Windows部署、实验报告与答辩演示。

## 9 持续协作

从main同一基线建立本角色feature分支，完成一个能编译的小改动就提交、推送、开PR给A。日1起持续合并，禁止第5天才首次拼接。改共享接口先提变更说明，A核对FG-API版本、B复核公共声明后再合并，全员Fetch/Pull更新；不要各自临时改返回类型或加同名模型。
