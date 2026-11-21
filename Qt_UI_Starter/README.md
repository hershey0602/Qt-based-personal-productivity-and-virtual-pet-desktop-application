# 拾光岛公共页面工程

本工程提供总览、任务、专注、宠物、统计、音乐和设置七个 Qt Widgets 页面，以及共享接口、公共组件、导航与服务装配。A 维护公共基础，B/C/D 在原文件接管业务实现。当前为第 1 天演示模式。

## 放在哪里

上传后，本文件夹 Qt_UI_Starter 与原有 FocusGarden 必须同级。CMake 默认从 ../FocusGarden 读取唯一资源和布局，不复制素材。

本机整理包中工程位于“整体框架/01_GitHub/Qt_UI_Starter”，CMake 也支持从这一位置找到外层共享资料。需要手动指定时，使用 FG_DESIGN_DIR 指向原 FocusGarden 目录。

## 上传到已有仓库

按同目录的 [加入已有GitHub仓库.md](加入已有GitHub仓库.md) 操作：先克隆或同步已有仓库，再复制新增文件、Commit、Push。

## 构建运行

Qt Creator 打开 CMakeLists.txt，选择统一的 Desktop Kit，Configure → Build → Run。本次队长本机已验证 Qt 6.11.2 / MinGW GCC 13.1.0 / x64 / CMake 3.30.5。团队实际统一值以 ENVIRONMENT_LOCK 为准。

在已有仓库根目录的 PowerShell 执行以下示例，工具路径按本机 Kit 调整：

```powershell
$env:PATH = 'C:/Qt/6.11.2/mingw_64/bin;C:/Qt/Tools/mingw1310_64/bin;C:/Qt/Tools/Ninja;' + $env:PATH
& C:/Qt/Tools/CMake_64/bin/cmake.exe -S Qt_UI_Starter -B build -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.11.2/mingw_64 -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
& C:/Qt/Tools/CMake_64/bin/cmake.exe --build build -j 4
& C:/Qt/Tools/CMake_64/bin/ctest.exe --test-dir build --output-on-failure
& ./build/FocusGarden.exe
```

本机查看整理包：双击“整体框架/02_Local/启动拾光岛.cmd”。实际程序和缓存放在 02_Local/build，截图和日志放在 02_Local/验收资料，这些均无需上传。

建议克隆目录使用 C:/Projects/FocusGarden 等英文路径，便于本机 MinGW 工具构建。

## 成员接管位置

| 成员 | 在这些原文件中继续开发 |
|---|---|
| A | MainWindow、AppComposition、DashboardPage、AppServices、CompletionCoordinator、contracts、公共 ui、顶层 CMake |
| B | TaskPage、FocusPage、StatisticsPage；Repository、TaskManager、FocusEngine、StatisticsService；TaskEditDialog、Migrations；tests/b |
| C | PetPage、RewardService、PetManager；tests/c |
| D | MusicPage、SettingsPage、MusicController、SettingsService、CompletionSound；tests/d、tests/integration、deployment |

共享声明只在 src/contracts；全部 86 个方法按 FG-API-1.0 生成。tools/audit_contracts.py 可对照 API_REGISTRY 复核声明和 45 项资源，默认报告写入 build/audit，可用 --output 指定位置。

MainWindow 只创建一组页面，QStackedWidget 切换页面。各页外套 QScrollArea。服务统一由 AppComposition 初始化一次，页面只保存门面的非拥有引用。LayoutPage 从冻结 JSON 一次构造布局；页面负责人在各自 cpp 连接业务行为。

默认使用 MockPorts，读取为空任务、0 金币、0 EXP、心情 70、25/5 默认设置和空音乐；业务写入返回 NotImplemented。A 在 AppComposition::initialize 接入成员真实服务后切换 Live。

真实服务装配顺序：SettingsService.load → Repository.open / recoverInterruptedSessions → 其余服务 → CompletionCoordinator → AppServices → 主窗口及七页 → loadPlaylist / retryPending。服务依赖与所有权按 FocusGarden/Qt_UI_Implementation_Blueprint/FILE_OWNERSHIP.json 和冻结契约执行。
