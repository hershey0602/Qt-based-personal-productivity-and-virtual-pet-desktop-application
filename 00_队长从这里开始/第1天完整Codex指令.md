# 队长第1天给Codex的完整指令

你是Focus Garden队长A的基础工程助手。当前输入是已整理的开工资料；请在这个仓库根目录创建唯一Qt_UI_Starter工程。本次Codex执行只完成日1公共基线并提交，不替B/C/D实现全部业务。完整项目第5天必须全部真实功能，日1占位绝不是日5验收成果；A之后继续执行自己的完整任务书。

先实际读取：
- 01_全员共同遵守的规则/ENVIRONMENT_LOCK.md
- FocusGarden/docs/SOURCE_PRD.txt
- FocusGarden/team/SHARED_AI_PROMPT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/DATA_MODELS.md
- FocusGarden/Qt_UI_Implementation_Blueprint/API_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/SIGNAL_SLOT_MATRIX.md
- FocusGarden/Qt_UI_Implementation_Blueprint/UI_BINDING_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/DATABASE_CONTRACT.md
- FocusGarden/Qt_UI_Implementation_Blueprint/SCHEMA_V1.sql
- FocusGarden/Qt_UI_Implementation_Blueprint/DEPENDENCY_MAP.md
- FocusGarden/Qt_UI_Implementation_Blueprint/PROJECT_STRUCTURE.md
- FocusGarden/Qt_UI_Implementation_Blueprint/FILE_OWNERSHIP.json
- FocusGarden/DesignTokens.json
- FocusGarden/UI_COMPONENTS.md
- FocusGarden/main.qss
- FocusGarden/resources.qrc
- 02_A队长开发任务/AI输入文件清单.md

如实际Qt版本、编译器/架构、Kit或模块未确认，先报告缺值，不能声称已构建。检查已有仓库内容，不覆盖无关文件。只建一个C++17 + Qt6 Widgets + CMake应用项目，不用WebEngine。

按FG-API-1.0一次生成src/contracts/DomainTypes.h与ServicePorts.h，所有类型/字段/枚举/签名照契约。AppServices和CompletionCoordinator由A实现转发与接线，不在四个模块各生成一次。创建AppComposition，并在应用启动只创建一组服务和七页；页面构造照UI_BINDING_CONTRACT。

建唯一MainWindow、七个页面入口和统一Sidebar/TopHeader；导航只QStackedWidget切换。日1各页可以是带正式标题/布局入口的占位，但保留对应类/对象名；不能把此占位当“全功能完成”。为B/C/D具体类和测试目录提供可编译声明/占位，public方法与冻结端口匹配；初始未实现写入返回NotImplemented，界面清楚标演示。

提供MockPorts作为日1/单元测试替身，默认空任务/0币/EXP0/心情70、默认设置25/5/音效开、空音乐；不实现或伪造真实奖励/计时。真实类文件按FILE_OWNERSHIP在基线发布后移交B/C/D。CMake提前登记这些文件，A统一维护构建配置，成员替换实现不另开项目。

公共组件至少提供可使用的卡片、主次危险/图标/小按钮、输入/下拉、状态标签、空错成功状态、计时圆环和七日图外壳，以及共用确认/结果框。TaskEditDialog业务由B完成；A只留入口与可编译占位。保持黄绿奶油白，不重设计。

资源只引用仓库../FocusGarden/resources.qrc，rcc相对该qrc解析UI_ASSETS等，禁止复制四份图片。运行加载:/fg/main.qss，SVG/PNG资源实际存在，字体按规范回退。布局用Layout，支持1600/1366/1280客户区，不全量绝对坐标。

建测试基础与A的壳导航/资源冒烟，Sql/Svg/Multimedia/Test依赖在同一Kit配置。实际执行配置、构建、运行七页导航；记录命令、退出码、环境和截图。环境不能运行时如实写未执行，不说已通过。

输出：完整同一工程源文件、构建与运行日志、七页切换截图、共享接口逐项对照、B/C/D文件接管清单、Mock切换真实类的AppComposition位置、未完成业务清单、Git提交说明。日1可编译后尽早提交基线SHA给队长发布GitHub；B/C/D据此同步开发完整功能。不要替后续成员提前写另一套接口或把数据业务留在假钱包里。
