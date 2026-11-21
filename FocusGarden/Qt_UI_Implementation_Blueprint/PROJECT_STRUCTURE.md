# 工程结构与装配冻结

FG-API-1.0。以下Qt_UI_Starter是下一次Codex待创建目标；本次只提供开工资料，当前没有CMakeLists.txt、C++头文件或可执行文件。

仓库根目录包含中文任务入口、FocusGarden/（唯一设计技术资料）和下一步创建的Qt_UI_Starter/（唯一软件工程）。让Codex打开整个仓库根目录，不能只上传自己角色文件后另建一个工程。

| 根目录 | 内容 | 是否提交GitHub |
|---|---|---|
| 00到08中文目录 | 使用说明、完整任务书、验收入口 | 是 |
| FocusGarden/ | Tokens、QSS、qrc、素材、原型、唯一接口和布局数据 | 是，只保留一套 |
| Qt_UI_Starter/ | 下一次Codex创建的C++17/Qt工程 | 是，创建后提交 |
| 本地归档/ | 上传原文和旧视觉来源 | 否，只供溯源 |
| build*/、运行数据库、打包输出 | 本机生成物 | 否 |

## 唯一工程内计划结构

Qt_UI_Starter/CMakeLists.txt为唯一应用项目入口。src/contracts是A生成的公共声明，src/services包括A门面/协调器、B存储任务计时统计、C奖励宠物、D设置音乐和完成短音效。七页全部位于src/pages；公共组件src/ui；TaskEditDialog归B。详细文件名单、唯一修改所有者与日1移交规则见FILE_OWNERSHIP.json，禁止靠目录名自行扩大权限。

CMake至少查找Qt6 Core、Widgets、Svg、Multimedia、Sql；测试查找Test。启用C++17、AUTOMOC、AUTORCC，选择Designer则AUTOUIC。目标为一个fg_ui共享库、一个FocusGarden应用、一个测试套件；B/C/D不能另外生成CMake项目。

资源只使用仓库FocusGarden/中的一份。A在CMake以CMAKE_CURRENT_LIST_DIR为基准计算../FocusGarden的绝对路径，把该目录resources.qrc加入唯一应用/公共库；rcc仍相对qrc所在目录解析UI_ASSETS、main.qss、DesignTokens。运行时读取:/fg/main.qss和:/fg/UI_ASSETS/...。不复制图片到四个模块、不在源码中使用本机C盘绝对路径。当前45项qrc引用不改名。

数据库DDL属于本技术蓝图SCHEMA_V1.sql，B在Migrations实现中按此生成/嵌入，不从部署后的源资料目录读取DDL。正式数据库与app.ini位于AppDataLocation，由A传完整路径，B/D分别读写。

## 装配顺序与依赖

A的AppComposition在应用启动时一次创建对象。Live顺序：设置组织/应用标识 → 创建AppData目录 → SettingsService.load → Repository.open/建表 → recoverInterruptedSessions → 创建TaskManager/StatisticsService/FocusEngine/RewardService/PetManager/MusicController/CompletionSound → CompletionCoordinator → AppServices → MainWindow/七页 → 连接一次信号 → loadPlaylist → retryPending。启动加载错误清楚呈现，不能伪装正式数据已经载入。

day1 Bootstrap可以全用MockPorts先确保能编译/导航；不代表阶段完成。B/C/D从同一基线同步实现真实类，各自测试；A在AppComposition逐个替换Mock，公开接口不变。第5天必须Live装配、真实SQLite与QSettings、真实倒计时和音频，Mock不能成为默认验收入口。

首阶段持续合并：日1基线尽早发布；各人当日开Draft PR；可以先合并能编译的独立部分，不能等全功能都完成才第一次合并。新源码CMake登记由A集中处理，日1提前预留文件避免四人同时改CMake。

## 页面接入

所有页构造签名与槽见UI_BINDING_CONTRACT。new七页和服务仅一次；导航只切QStackedWidget。AppServices聚合成功信号并排队刷新，页面不操作SQL、不新建计时器/播放器、不维护钱包。D拥有SettingsPage与SettingsService，A只注册导航、注入与整合；B仅读取默认配置。
