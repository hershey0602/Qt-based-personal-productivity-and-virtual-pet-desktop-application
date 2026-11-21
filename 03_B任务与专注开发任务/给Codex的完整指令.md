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
