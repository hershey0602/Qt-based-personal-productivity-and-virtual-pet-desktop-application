# 给Codex的完整指令 角色C

你现在是Focus Garden四人小组的C成员，负责宠物 金币 喂食 成长与持久化对接。这是在队长已经建立的同一Qt工程内的一次完整任务，不是只做静态UI。第5天前必须交付本文全部真实功能，数据持久化和异常分支一并实现。第6天后仅允许测试、修复、优化、部署和报告答辩，不把任何必做业务留到后续再做。

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
- FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md
- FocusGarden/Qt_UI_Implementation_Blueprint/PENDING_AND_DECISIONS.md
- FocusGarden/UI_完整开发规范.docx
- FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html
- FocusGarden/Qt_UI_Implementation_Blueprint/DATABASE_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/SCHEMA_V1.sql
- FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/pet.json
- FocusGarden/UI_ASSETS/ASSET_MANIFEST.csv
- FocusGarden/七个页面与弹窗的组件参数表.xlsx

在A发布后额外读取：

- Qt_UI_Starter/CMakeLists.txt
- Qt_UI_Starter/src/contracts/DomainTypes.h
- Qt_UI_Starter/src/contracts/ServicePorts.h
- Qt_UI_Starter/src/services/AppServices.h
- Qt_UI_Starter/src/ui/Components.h
- Qt_UI_Starter/src/AppComposition.h
- Qt_UI_Starter/src/pages/PetPage.h
- Qt_UI_Starter/src/services/RewardService.h
- Qt_UI_Starter/src/services/PetManager.h

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

- 真实金币收入：仅B已保存的自然完成Focus/Custom可结算，每5分钟4币；25分钟20；任务完成、休息、放弃、未保存会话零币。收入与sessionId关联并有唯一保护。
- 奖励失败和幂等：实现settleCompletedSession并调用B Repo原子端口；重复结算返回原金额且newlyCredited=false，不重复发币/到账提示。失败留Pending供A协调重试。
- 唯一钱包与流水：从Repository读取ledger合计与pet_state；不另开库，不在QSettings或页面维护余额。余额、流水、奖励状态、宠物经验重启后保持。
- 喂食原子操作：余额<15只提示不变化；足够时一个事务扣15、累计EXP+10、心情+15上限100。一次操作OperationId，失败回滚，重复相同操作不重扣。
- 等级与宠物UI：布丁Lv1～3真实PNG，初始EXP0/心情70/币0；累计50/150升级不重置经验，升级用A结果框；Lv3仍可喂食但无Lv4及虚假进度。无衰减、抚摸、商店、背包。
- 跨模块刷新：提交成功发冻结信号，A总览/专注结果与C页同源更新；本模块不能通过正式UI模拟加币按钮交差。错误不改快照不伪报成功。

允许编辑的计划文件或目录如下（A的初始占位在发布基线后按所有权移交）：

- `Qt_UI_Starter/src/pages/PetPage.h`
- `Qt_UI_Starter/src/pages/PetPage.cpp`
- `Qt_UI_Starter/src/services/RewardService.h`
- `Qt_UI_Starter/src/services/RewardService.cpp`
- `Qt_UI_Starter/src/services/PetManager.h`
- `Qt_UI_Starter/src/services/PetManager.cpp`
- `Qt_UI_Starter/tests/c/`

修改限制：禁止修改CMakeLists.txt、AppComposition、MainWindow、src/contracts、AppServices、FocusGarden公共QSS/Token/qrc及他人实现。新增构建登记/接口变化交A做最小修改；可改本人类私有实现和测试，但公共override签名不变。

按这个顺序实际开发，不只给计划：

1. 先读类型、奖励/宠物端口和Repository原子事务合同；从A基线创建自己的分支，不另起项目。
2. 可用Fake IRepository做独立规则测试，同时与B确认日1原子端口交付；替身测试不是日5验收。
3. 实现RewardService检查真实Completed会话，调用creditRewardAtomically并保持sessionId幂等；与A Coordinator对接。
4. 实现PetManager.feed与真实ledger/pet_state事务，完成不足/失败/重复操作边界。
5. 按Pet布局完成静态资源、等级/心情/金币/经验，复用A结果框与进度。
6. 日3起联调B自然完成记录，日5用真实记录、重启和失败回滚证明C功能全部完成。

准确实现IRewardService, IPetService在API_CONTRACT中的全部方法，按DATA_MODELS生成/使用字段与枚举，按SIGNAL_SLOT_MATRIX触发信号。你的页面构造、公共组件与模态遵守UI_BINDING_CONTRACT。B/C/D具体服务继承端口的signals，不重复声明；A具体AppServices和CompletionCoordinator按契约在本类声明signals一次，不复制门面。Repository原子事务与唯一索引遵守DATABASE_CONTRACT。

完成前逐项操作并记录：

- C-T01 首次空库进入宠物，0币点击喂食 → Lv1/EXP0/心情70；提示不足，所有数值不变。
- C-T02 B真实完成25分钟，再查询ledger/余额 → 只新增+20收入，对应真实sessionId。
- C-T03 对同一个sessionId结算两次/重启后再重试 → 仅一收入行；余额只加一次，newlyCredited=false不重复提示。
- C-T04 传Running/Paused/Abandoned/不存在ID或休息 → 无收入；清楚返回错误，不创建伪完成记录。
- C-T05 已有20币喂食一次，再重启 → 余额5、EXP+10、心情+15（最高100），结果均保留。
- C-T06 在事务中制造失败或重复同OperationId → 流水/EXP/心情一起回滚；重复命令不重复扣15。
- C-T07 测试EXP40→50、140→150及满级后继续喂食 → 升级2/3，经验累计；无Lv4；满级隐藏升级条，仍正确扣币和增长。
- C-T08 结算失败后重试、任务完成或放弃专注 → 失败不加币，重试一次；完成任务/放弃均0币。
- C-T09 喂食或到账后不重启切总览、宠物、统计历史 → 读同一余额/记录，无页面私有钱包，A收到统一刷新。

必须输出：

- 源代码：在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。
- 业务与接口：列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。
- 构建与测试：真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。
- 运行证据：本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。
- 验收记录：逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。
- 已知问题：重现步骤、影响、原因或待查、负责人、修复提交/复测状态。
- Git交付：推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。

额外提交按sessionId查重记录、账本合计核对、喂食回滚/OperationId测试及50/150等级边界截图。

把证据放到08_测试验收与交付清单/evidence/C/，不要把正式用户数据库、账号凭据或无权分发的歌曲加入仓库。提交说明写需求ID、实现文件、命令/退出码、截图路径、未完成项和公共接口是否改变。发现接口冲突先停在冲突边界并提交最小变更说明交A审核；其余不依赖部分可继续，不用兼容包装另造接口。

本任务是一次性全范围布置，各日程是进度检查点，不需要队长每天重新发任务。没有真实代码、构建/运行结果和功能证据不能写已完成；测试环境不支持的检查列未执行，交组长判断，不能用模拟页冒充全功能软件。
