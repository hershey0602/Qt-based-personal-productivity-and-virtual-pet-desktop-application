# D_音乐设置测试与部署_完整任务书

你负责音乐 设置 测试 部署。这是从开工到最后交付的一次性完整任务。D-T12对应第22～28天部署验收；日5已须具备全部真实业务功能，不能借此延期播放或设置实现。前5天实现全部分配功能与真实持久化，后续只测试修复优化部署答辩；Mock仅日1支架/测试替身。SettingsPage与SettingsService由D实现，A只装配与统一UI。功能以有效PRD、视觉以UI V2.1、接口以FG-API-1.0为准。

## 1 我负责什么

| 编号 | 范围 | 必做内容 |
|---|---|---|
| D01 | 本地音乐真实播放 | 多文件导入、单击选中/双击播放、播放暂停停止、上一首下一首两端循环、Seek、音量、移除；QMediaPlayer+QAudioOutput唯一实例，跨页继续。首次空列表，不内置歌单，不自动播放。 |
| D02 | 媒体错误和路径保存 | 导入/移除不删除磁盘文件；非当前曲移除不打断；当前曲移除停止并选相邻合法项。损坏/失效路径清楚反馈；全清单无有效文件时停止，不能无限循环错误。保存本地路径/音量，重启不自播。 |
| D03 | SettingsPage与SettingsService | D同时负责页面和服务：默认专注25/50、休息5/10、完成音效开关；本地数据与关于只读。无保存按钮、主题/账号/导出，逐次修改即时持久化。 |
| D04 | 设置失败与跨模块 | QSettings固定AppData app.ini，sync/status成功后发布preferencesChanged；失败恢复旧值并提示。B当前活动轮计划不变，下一轮采用新值；显式时长不被导航覆盖。 |
| D05 | 完成提示音 | CompletionSound加载真实原创短音效；B自然完成记录保存后按设置播放，sessionId去重；关闭开关不播放，不能自动控制/启动音乐。 |
| D06 | 全组测试和部署 | 日1起准备验收表/测试基础，各人自测；日5统筹全部真实功能验收。日6后只做回归、Bug修复、视觉/性能/稳定性优化、Windows打包、新电脑运行测试和答辩资料。 |

你主责的PRD/验收ID为：NAV-16, NAV-17, MUSIC-01, MUSIC-02, MUSIC-03, MUSIC-04, MUSIC-05, MUSIC-06, MUSIC-07, MUSIC-08, MUSIC-09, SET-01, SET-02, SET-03, SET-04, SET-05, AC-20, AC-21, AC-27, AC-29, AC-30, AC-34。全量映射在08目录PRD覆盖与责任矩阵.csv；协助他人不表示另造同一接口。

## 2 我要从哪里开始

1. 拿A基线，先完成SettingsService真实load/save和三项配置，日1优先提供给B读取默认值；A仅装配，不由A另实现设置。
2. 同时搭Music/Settings页面，复用公共控件和精确布局；直接调用冻结接口，不另写Task/Preferences结构。
3. 完成真实QMediaPlayer/QAudioOutput与清单管理、位置/音量、错误与恢复，应用级持有不随页面析构。
4. 把D完成音效与A/B信号连接，验证默认值下一轮生效、当前轮不变、失败回旧值。
5. 日4前补齐全部媒体和设置异常；日5先完成自己全部真实功能，再统筹全功能清单与记录。
6. 日6以后扩展回归/耐久/多机部署、修复已有功能缺陷、整理报告答辩；不能把设置持久化或真实播放留到日6才首次开发。

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
| `FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md` | 构造依赖与禁止循环 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/PENDING_AND_DECISIONS.md` | 确认已定产品规则和真实技术阻塞 |
| `FocusGarden/UI_完整开发规范.docx` | 七页及弹窗完整视觉规范，保留V2.1 |
| `FocusGarden/prototype/FocusGarden_Interactive_Prototype_V2.1.html` | 完整可点击原型；只能借鉴视觉交互，不把假数据当后端 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/STATE_MACHINES.md` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/music.json` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/layouts/settings.json` | 本模块重点读取 |
| `FocusGarden/UI_ASSETS/audio/sfx_focus_complete.wav` | 本模块重点读取 |
| `FocusGarden/Qt_UI_Implementation_Blueprint/ACCEPTANCE_PLAN.md` | 本模块重点读取 |
| `FocusGarden/七个页面与弹窗的组件参数表.xlsx` | 本模块重点读取 |
| `08_测试验收与交付清单/第5天全功能验收清单.md` | 本模块重点读取 |

A发布基线后，再读取以下真实工程文件：

- `Qt_UI_Starter/CMakeLists.txt`
- `Qt_UI_Starter/src/contracts/DomainTypes.h`
- `Qt_UI_Starter/src/contracts/ServicePorts.h`
- `Qt_UI_Starter/src/services/AppServices.h`
- `Qt_UI_Starter/src/ui/Components.h`
- `Qt_UI_Starter/src/AppComposition.h`
- `Qt_UI_Starter/src/pages/MusicPage.h`
- `Qt_UI_Starter/src/pages/SettingsPage.h`
- `Qt_UI_Starter/src/services/MusicController.h`
- `Qt_UI_Starter/src/services/SettingsService.h`
- `Qt_UI_Starter/src/services/CompletionSound.h`

日1之前这些Qt文件尚未生成；成员拿到基线后若文件缺失，报A修复公共壳，不创建第二工程。输入清单机器版见同目录AI_INPUT_FILES.json。

## 4 文件修改权限

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

禁止修改CMakeLists.txt、AppComposition、MainWindow、src/contracts、AppServices、FocusGarden公共QSS/Token/qrc及他人实现。新增构建登记/接口变化交A做最小修改；可改本人类私有实现和测试，但公共override签名不变。

## 5 精确接口与数据

所有数据结构统一引用DATA_MODELS：Task/TaskDraft/TaskFilter、FocusConfig/FocusSnapshot/FocusSession、Result/OperationResult、Preferences、PetSnapshot、统计/媒体快照与receipt。ID和时间口径不得另起定义。以下摘录由同一API注册表生成，遇版本差异以冻结API_CONTRACT为准：


### IMusicController

| 必须实现的准确签名 | 调用与语义 |
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

继承使用的信号（不得在具体类重复声明）：

- `void musicChanged(fg::MusicSnapshot snapshot);`
- `void errorOccurred(fg::AppError error);`

### ISettingsService

| 必须实现的准确签名 | 调用与语义 |
|---|---|
| `OperationResult load();` | 启动一次；损坏/失败明确报错，内存采用默认值但不冒称已保存 |
| `Preferences preferences() const;` | B/A读取已加载缓存 |
| `OperationResult savePreferences(const Preferences& preferences);` | D设置页经门面调用；sync/status失败恢复旧缓存和旧键并报错 |
| `MusicPreferences musicPreferences() const;` | D MusicController读取清单与音量 |
| `OperationResult saveMusicPreferences(const MusicPreferences& preferences);` | D MusicController保存本地路径与音量，不新增设置页选项 |

继承使用的信号（不得在具体类重复声明）：

- `void preferencesChanged(fg::Preferences preferences);`
- `void musicPreferencesChanged(fg::MusicPreferences preferences);`
- `void errorOccurred(fg::AppError error);`

### ICompletionSound

| 必须实现的准确签名 | 调用与语义 |
|---|---|
| `void playForCompletedSession(const SessionId& sessionId);` | A连接B已保存自然完成信号；读取开关，按sessionId抑制重复音效 |

继承使用的信号（不得在具体类重复声明）：

- `void errorOccurred(fg::AppError error);`


页面signals/slots逐字见SIGNAL_SLOT_MATRIX；UI构造/槽逐字见UI_BINDING_CONTRACT。A的门面统一转发，B/C/D具体类只实现本人端口。写入失败返回中文错误，不能伪报成功或关闭需重试的窗口。

## 6 可直接复制的完整开发指令

下面这份内容可整段交给Codex；同目录给Codex的完整指令.md是方便复制的纯文本版。

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


## 7 AI完成后必须交什么

# D 的代码与测试交付清单

| 交付项 | 必须包含什么 |
|---|---|
| 源代码 | 在同一Qt_UI_Starter中提交本人.h/.cpp、必要.ui与接入代码；不能只交另一个项目ZIP。 |
| 业务与接口 | 列出实现的端口方法、共享信号、持久化或读写路径；全部按FG-API-1.0。 |
| 构建与测试 | 真实Qt/编译器/Kit/提交SHA、配置与构建日志、测试命令/退出码；没执行写未执行。 |
| 运行证据 | 本人页面/弹窗/异常与三尺寸截图，业务链路日志；演示数据不能冒充Live。 |
| 验收记录 | 逐功能通过/失败/未执行、证据路径；缺项明确写未完成，不口头说“好了”。 |
| 已知问题 | 重现步骤、影响、原因或待查、负责人、修复提交/复测状态。 |
| Git交付 | 推送本人分支，开小PR到main，说明改哪些文件、为什么、怎样验证，A合并。 |

额外提交真实听音/Seek用例、设置重启与失败日志、全组验收台账；后续交Windows发布目录、另一台电脑运行证据、缺陷回归和答辩演示清单。

| 编号 | 实际操作 | 完成标准 |
|---|---|---|
| D-T01 | 首次空清单导入2～3首合法本地音频 | 加入不自播；单击选择不自播，双击有真实声音。 |
| D-T02 | 播放→暂停→继续→停止，切任务/宠物再回来 | 状态/进度真实；暂停冻结，停止归零，跨页继续播放。 |
| D-T03 | 上一首/下一首和自然播完末曲 | 按顺序两端循环，播放时换曲继续播，无有效曲停止。 |
| D-T04 | 拖动可seek媒体、调0/100音量 | 真实位置/音量改变，不seekable禁用并明确返回错误。 |
| D-T05 | 移除非当前项/当前项，移动或破坏文件 | 磁盘文件不删；其他曲继续；当前项停止；坏路径只一条合理错误且不闪退。 |
| D-T06 | 保存清单和音量后重启 | 路径/配置恢复，不自动播放，无假歌曲或联网依赖。 |
| D-T07 | 设置25→50、5→10、关闭提示音，重启 | 值立即落盘并保持，设置页与服务一致。 |
| D-T08 | B运行25分钟期间改50，再手动建下一轮 | 当前计划仍25，下一轮50；休息默认同理。 |
| D-T09 | 注入设置写入失败、自然完成触发提示音 | 失败回旧值且不发成功信号；音效开关有效，重复session不重复播放。 |
| D-T10 | 查看设置页和负责文件清单 | 只有三项可改，SettingsPage/SettingsService均由D提交，A只导航/整合。 |
| D-T11 | 日5执行全组功能清单；日6后重复验收 | 每项真实结果有证据，无用Mock通过；缺陷标明责任与复测。 |
| D-T12 | 第22～28天：Release打包后在未装Qt另一台Windows电脑运行 | 平台/SVG/SQLite/音频所需依赖齐，图像加载和真实本地音频可用；提交实际部署日志。 |


## 8 五天内的检查点

| 日程 | 截止检查点 |
|---|---|
| 日1 | SettingsService真实默认值和持久化优先给B；建立全组验收表，音乐设置页面开始接公共壳。 |
| 日2 | 本地导入、真实播放、基本控制和设置页即时保存。 |
| 日3 | 清单/音量恢复、进度切歌、音效与B下一轮配置联调。 |
| 日4 | 坏文件/无有效曲/回滚/当前轮不变等全部异常和页面完成。 |
| 日5 | D全功能真实验收并组织全组日5清单；后续只回归修复优化部署答辩。 |

以上检查点不缩减第5天全功能目标。失败/未完成必须如实列明，不改验收口径，不安排第二轮必做功能开发。第6～28天按全组计划只做运行验收、Bug修复、视觉/性能稳定性、Windows部署、实验报告与答辩演示。

## 9 持续协作

从main同一基线建立本角色feature分支，完成一个能编译的小改动就提交、推送、开PR给A。日1起持续合并，禁止第5天才首次拼接。改共享接口先提变更说明，A核对FG-API版本、B复核公共声明后再合并，全员Fetch/Pull更新；不要各自临时改返回类型或加同名模型。
