# 给Codex的完整指令 角色D

你现在是Focus Garden四人小组的D成员，负责音乐 设置 测试 部署。这是在队长已经建立的同一Qt工程内的一次完整任务，不是只做静态UI。第5天前必须交付本文全部真实功能，数据持久化和异常分支一并实现。第6天后仅允许测试、修复、优化、部署和报告答辩，不把任何必做业务留到后续再做。

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
- FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md
- FocusGarden/Qt_UI_Implementation_Blueprint/PENDING_AND_DECISIONS.md
- FocusGarden/UI_完整开发规范.docx
- FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html
- FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/music.json
- FocusGarden/Qt_UI_Implementation_Blueprint/layouts/settings.json
- FocusGarden/UI_ASSETS/audio/sfx_focus_complete.wav
- FocusGarden/Qt_UI_Implementation_Blueprint/ACCEPTANCE_PLAN.md
- FocusGarden/七个页面与弹窗的组件参数表.xlsx
- 08_测试验收与交付清单/第5天全功能验收清单.md

在A发布后额外读取：

- Qt_UI_Starter/CMakeLists.txt
- Qt_UI_Starter/src/contracts/DomainTypes.h
- Qt_UI_Starter/src/contracts/ServicePorts.h
- Qt_UI_Starter/src/services/AppServices.h
- Qt_UI_Starter/src/ui/Components.h
- Qt_UI_Starter/src/AppComposition.h
- Qt_UI_Starter/src/pages/MusicPage.h
- Qt_UI_Starter/src/pages/SettingsPage.h
- Qt_UI_Starter/src/services/MusicController.h
- Qt_UI_Starter/src/services/SettingsService.h
- Qt_UI_Starter/src/services/CompletionSound.h

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

- 本地音乐真实播放：多文件导入、单击选中/双击播放、播放暂停停止、上一首下一首两端循环、Seek、音量、移除；QMediaPlayer+QAudioOutput唯一实例，跨页继续。首次空列表，不内置歌单，不自动播放。
- 媒体错误和路径保存：导入/移除不删除磁盘文件；非当前曲移除不打断；当前曲移除停止并选相邻合法项。损坏/失效路径清楚反馈；全清单无有效文件时停止，不能无限循环错误。保存本地路径/音量，重启不自播。
- SettingsPage与SettingsService：D同时负责页面和服务：默认专注25/50、休息5/10、完成音效开关；本地数据与关于只读。无保存按钮、主题/账号/导出，逐次修改即时持久化。
- 设置失败与跨模块：QSettings固定AppData app.ini，sync/status成功后发布preferencesChanged；失败恢复旧值并提示。B当前活动轮计划不变，下一轮采用新值；显式时长不被导航覆盖。
- 完成提示音：CompletionSound加载真实原创短音效；B自然完成记录保存后按设置播放，sessionId去重；关闭开关不播放，不能自动控制/启动音乐。
- 全组测试和部署：日1起准备验收表/测试基础，各人自测；日5统筹全部真实功能验收。日6后只做回归、Bug修复、视觉/性能/稳定性优化、Windows打包、新电脑运行测试和答辩资料。

允许编辑的计划文件或目录如下（A的初始占位在发布基线后按所有权移交）：

- `Qt_UI_Starter/src/pages/MusicPage.h`
- `Qt_UI_Starter/src/pages/MusicPage.cpp`
- `Qt_UI_Starter/src/pages/SettingsPage.h`
- `Qt_UI_Starter/src/pages/SettingsPage.cpp`
- `Qt_UI_Starter/src/services/MusicController.h`
- `Qt_UI_Starter/src/services/MusicController.cpp`
- `Qt_UI_Starter/src/services/SettingsService.h`
- `Qt_UI_Starter/src/services/SettingsService.cpp`
- `Qt_UI_Starter/src/services/CompletionSound.h`
- `Qt_UI_Starter/src/services/CompletionSound.cpp`
- `Qt_UI_Starter/tests/d/`
- `Qt_UI_Starter/tests/integration/`
- `Qt_UI_Starter/deployment/`

修改限制：禁止修改CMakeLists.txt、AppComposition、MainWindow、src/contracts、AppServices、FocusGarden公共QSS/Token/qrc及他人实现。新增构建登记/接口变化交A做最小修改；可改本人类私有实现和测试，但公共override签名不变。

按这个顺序实际开发，不只给计划：

1. 拿A基线，先完成SettingsService真实load/save和三项配置，日1优先提供给B读取默认值；A仅装配，不由A另实现设置。
2. 同时搭Music/Settings页面，复用公共控件和精确布局；直接调用冻结接口，不另写Task/Preferences结构。
3. 完成真实QMediaPlayer/QAudioOutput与清单管理、位置/音量、错误与恢复，应用级持有不随页面析构。
4. 把D完成音效与A/B信号连接，验证默认值下一轮生效、当前轮不变、失败回旧值。
5. 日4前补齐全部媒体和设置异常；日5先完成自己全部真实功能，再统筹全功能清单与记录。
6. 日6以后扩展回归/耐久/多机部署、修复已有功能缺陷、整理报告答辩；不能把设置持久化或真实播放留到日6才首次开发。

准确实现IMusicController, ISettingsService, ICompletionSound在API_CONTRACT中的全部方法，按DATA_MODELS生成/使用字段与枚举，按SIGNAL_SLOT_MATRIX触发信号。你的页面构造、公共组件与模态遵守UI_BINDING_CONTRACT。B/C/D具体服务继承端口的signals，不重复声明；A具体AppServices和CompletionCoordinator按契约在本类声明signals一次，不复制门面。Repository原子事务与唯一索引遵守DATABASE_CONTRACT。

完成前逐项操作并记录：

- D-T01 首次空清单导入2～3首合法本地音频 → 加入不自播；单击选择不自播，双击有真实声音。
- D-T02 播放→暂停→继续→停止，切任务/宠物再回来 → 状态/进度真实；暂停冻结，停止归零，跨页继续播放。
- D-T03 上一首/下一首和自然播完末曲 → 按顺序两端循环，播放时换曲继续播，无有效曲停止。
- D-T04 拖动可seek媒体、调0/100音量 → 真实位置/音量改变，不seekable禁用并明确返回错误。
- D-T05 移除非当前项/当前项，移动或破坏文件 → 磁盘文件不删；其他曲继续；当前项停止；坏路径只一条合理错误且不闪退。
- D-T06 保存清单和音量后重启 → 路径/配置恢复，不自动播放，无假歌曲或联网依赖。
- D-T07 设置25→50、5→10、关闭提示音，重启 → 值立即落盘并保持，设置页与服务一致。
- D-T08 B运行25分钟期间改50，再手动建下一轮 → 当前计划仍25，下一轮50；休息默认同理。
- D-T09 注入设置写入失败、自然完成触发提示音 → 失败回旧值且不发成功信号；音效开关有效，重复session不重复播放。
- D-T10 查看设置页和负责文件清单 → 只有三项可改，SettingsPage/SettingsService均由D提交，A只导航/整合。
- D-T11 日5执行全组功能清单；日6后重复验收 → 每项真实结果有证据，无用Mock通过；缺陷标明责任与复测。
- D-T12【第22～28天部署验收，非日5门槛】Release打包后在未装Qt另一台Windows电脑运行 → 平台/SVG/SQLite/音频所需依赖齐，图像加载和真实本地音频可用；提交实际部署日志。

必须输出：

- 源代码：在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。
- 业务与接口：列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。
- 构建与测试：真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。
- 运行证据：本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。
- 验收记录：逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。
- 已知问题：重现步骤、影响、原因或待查、负责人、修复提交/复测状态。
- Git交付：推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。

额外提交真实听音/Seek用例、设置重启与失败日志、全组验收台账；后续交Windows发布目录、另一台电脑运行证据、缺陷回归和答辩演示清单。

把证据放到08_测试验收与交付清单/evidence/D/，不要把正式用户数据库、账号凭据或无权分发的歌曲加入仓库。提交说明写需求ID、实现文件、命令/退出码、截图路径、未完成项和公共接口是否改变。发现接口冲突先停在冲突边界并提交最小变更说明交A审核；其余不依赖部分可继续，不用兼容包装另造接口。

本任务是一次性全范围布置，各日程是进度检查点，不需要队长每天重新发任务。没有真实代码、构建/运行结果和功能证据不能写已完成；测试环境不支持的检查列未执行，交组长判断，不能用模拟页冒充全功能软件。
