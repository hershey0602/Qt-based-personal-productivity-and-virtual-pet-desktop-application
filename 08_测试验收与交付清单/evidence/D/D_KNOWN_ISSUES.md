# D 已知问题

记录时间：2026-11-27　分支：`feature/D-music-settings`

| # | 问题 | 重现步骤 | 影响 | 原因 / 待查 | 负责人 | 状态 |
|---|---|---|---|---|---|---|
| 1 | 基线为 Demo(Mock) 装配，D 的实现未被实例化 | 运行 `FocusGarden.exe`，进音乐页点“添加本地音乐”，选任意 wav | 所有 UI 级验收无法进行，D-T02/05/07/10 只能停留在服务级 | `main.cpp` 默认 `ModelMode::Demo`，`AppServices` 注入 `MockPorts` | A | 待 A 接入 |
| 2 | `--live` 通道不可用 | 运行 `FocusGarden.exe --live` | 弹“当前为第1天公共框架，请以演示模式运行”，Live 路径完全不可达 | `AppComposition.cpp:7` Live 分支写死返回 `NotImplemented` | A | 待 A 实现 |
| 3 | `tests/d` 未登记进根构建 | 直接 `cmake -S Qt_UI_Starter -B build`，`fg_tests` 不含 D 用例 | D 用例不进主 CI，只能靠独立工程手工跑 | 根 `CMakeLists.txt` 归 A；登记说明见 `Qt_UI_Starter/tests/d/BUILD_REGISTRATION_CHANGE.md` | A | 待 A 登记 |
| 4 | 仓库根缺 `.gitignore` | `git status` 可见 `build/` 下 460+ 个未跟踪文件 | 误用 `git add .` 会把构建产物提交进历史 | 仓库级配置缺失，且不在任何人的归属清单内 | A（建议） | 待处理 |
| 5 | MinGW 编译器在缺少 PATH 时静默失败 | 不设置 PATH，直接调用 `g++.exe -c x.cpp` | `cc1plus.exe` 返回 `0xC0000135`（DLL 缺失），g++ 退出 1 且**无任何错误输出**，极易误判为代码错误 | `cc1plus` 依赖 `C:\Qt\Tools\mingw1310_64\bin` 下的运行库；需把该目录加入 PATH | D（已在验证脚本中处理） | 已规避 |
| 6 | CMake 生成器参数含空格易被拆开 | `cmake -S ... -B ... -G MinGW Makefiles` 未加引号 | 报 `Ignoring extra path from command line: "Makefiles"`，配置退出码 1 | 参数需作为单个 token 传入（`-G "MinGW Makefiles"`） | D（已在文档中固化） | 已规避 |
| 7 | 验证工程曾误纳入他人源码导致链接失败 | 源清单含 `MusicPage/Components/AppServices/CompletionCoordinator` | 链接期 19 个未定义符号（`IPetService`、`IFocusEngine`、`ISettingsService` 的 `qt_metacast/qt_metacall`） | 这些类依赖别的成员实现与 moc 产物；服务级验证只需 D 的三个服务 + `ServicePorts.h` | D | 已修复 |
| 8 | 证据缺口：窗口尺寸截图与跨机运行 | 查看 `evidence/D/` | 交付清单“三尺寸截图”“另一台电脑运行”未满足 | 需人工在不同窗口尺寸下截图；需另一台未装 Qt 的机器 | D | 未完成 |
| 9 | 音频格式与边界场景覆盖不全 | 导入 `.flac` / `.m4a` / 超长文件，或拔掉所有音频输出设备后播放 | 可能在这些场景下出现解码失败、进度异常或无提示 | 当前自动化只覆盖 `.wav` 与 `.mp3`；其余格式依赖 FFmpeg 后端，未单独验证 | D | 未覆盖（已在验收记录中声明） |

## 备注

- 问题 1、2 是同一根因的两个面：装配层没有切到真实实现。A 完成后需重跑 `D_ACCEPTANCE.md` 中全部“未执行/部分”项。
- 问题 5 的失败形态具有强误导性（无声退出），后续任何人本地构建 D 验证工程前都应先确认 PATH。
