# B 的AI输入文件清单

在Codex打开整个仓库根目录；不是把文件逐个复制出四份。下表是现在必须让AI读取的精确路径。

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

## A发布公共工程后必须读取

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

这些Qt文件目前是下一步待创建目标。A未发布时先审阅合同和自身任务，不另起一个工程；A发布后缺任何公共头文件/构建入口就列技术阻塞。必须记录基线SHA，先构建原基线再修改。
