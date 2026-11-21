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
