# Focus Garden 四人统一 Qt 环境测试

四人使用同一 Git 提交中的 **整个 `tools/QtEnvCheck/` 目录**。这是独立 CMake 工程，入口为本目录 `CMakeLists.txt`。

## 统一环境

| 项目 | 必须使用的值 |
|---|---|
| 系统 | Windows 10 / 11，64 位 |
| Qt | 6.11.2，MinGW 64 位版本 |
| 编译器 | MinGW GCC 13.1.0，64 位 |
| CMake | 3.30.5 |
| C++ 标准 | C++17 |
| Qt 模块 | Widgets、Svg、Multimedia、Sql、Test |

CMake 对 Qt 使用 `6.11.2 EXACT REQUIRED`，并分别校验模块版本、编译器版本、64 位架构与 CMake 版本。环境不符会终止配置并显示原因。各人的安装目录可以不同，通过本机 Kit 选择工具链。

## 在自己电脑上依次点击

1. 在 GitHub 仓库点击 **Code → Download ZIP**，解压到纯英文路径，例如 `D:/Projects/FocusGarden`；也可以用 GitHub Desktop 克隆到此类路径。四人核对同一个提交 SHA。
2. 打开 **Qt Creator → 文件 → 打开文件或项目**，选择 `tools/QtEnvCheck/CMakeLists.txt`，点击 **打开**。
3. 在 Kit 选择页勾选 **Desktop Qt 6.11.2 MinGW 64-bit**。具体显示名称可能随安装不同，须核对 Qt 与编译器实际版本。
4. 点击 **编辑 → Preferences（首选项）→ Kits**；部分 Creator 版本入口叫 **工具 → 选项 → Kits**。在 Qt Versions 中选择 Qt 6.11.2 的 qmake；在 Compilers 中选择 MinGW GCC 13.1.0 的 C++ 编译器；在 CMake 中添加/选择 3.30.5。回到 Kits，把这三项绑定到同一个 Desktop Kit。编译器 ABI 应为 x86 / Windows / MinGW / 64 位。
5. 点击左侧 **项目 → 构建设置**，选择 **Debug**。把构建目录设到纯英文路径，例如 `D:/Build/QtEnvCheck`，点击 **Configure Project / 配置项目**。在 CMake 输出中确认 `Environment locked: Qt 6.11.2, GCC 13.1.0, CMake 3.30.5, C++17, x64`。
6. 点击左下角 **锤子（构建）** 或按 **Ctrl+B**。在编译输出中确认构建完成。
7. 在左下角运行目标选择 **QtEnvCheck**，点击 **绿色三角（运行）** 或按 **Ctrl+R**。
8. 查看窗口：编译 Qt 和运行 Qt 都应为 **6.11.2**，显示绿色 SVG 圆形勾号，Widgets、Svg、Sql / SQLite、Multimedia、Test 五行均为 **PASS**，底部为 **PASS：5/5**。任一 FAIL 都会在该行给出原因。
9. 运行自动化测试：点击 **工具 → 测试 → 运行所有测试**；在测试结果面板核对 `QtEnvCheck.Unit` 和 `QtEnvCheck.Window`。若当前 Creator 没有显示 CTest 项，在 **帮助 → 关于插件** 中启用 CMakeProjectManager / AutoTest 并重新打开工程，或直接按下一节执行 CTest。
10. 复制 `TEST_REPORT_TEMPLATE.md` 为自己的报告，填写成员、Kit、版本、提交 SHA、两组测试结果，附窗口截图与本机日志。A/B/C/D 各自运行一遍。

中文构建路径在本次机器上触发过 moc 输出文件创建失败，使用纯英文仓库及构建路径即可按上述流程构建。

## CTest 命令与日志

在 Qt Creator 的构建设置中复制实际构建目录，使用所选 CMake 3.30.5 同目录的 `ctest.exe`。下列变量填写自己的路径；不复制其他成员的安装目录。

```powershell
$CMakeBin = Read-Host 'CMake 3.30.5 的 bin 目录'
$BuildDir = Read-Host 'Qt Creator 的构建目录'
& "$CMakeBin/cmake.exe" --build $BuildDir --parallel 4
& "$CMakeBin/ctest.exe" --test-dir $BuildDir --verbose --output-on-failure 2>&1 |
    Tee-Object ./ctest-local.log
if ($LASTEXITCODE -ne 0) { throw '环境测试 FAIL，请检查日志' }
```

CTest 会从所选 Kit 自动推导 Qt DLL 和 MinGW 运行库目录。成功应显示 `100% tests passed, 0 tests failed out of 2`。GUI 自检使用 `--self-test`，真正打开桌面窗口、等待显示、执行五项检查，并以 0（全部 PASS）或 1（存在 FAIL）退出。

命令行独立配置也可以使用以下方式。在当前目录打开 PowerShell，填写本机路径：

```powershell
$QtPrefix = Read-Host 'Qt 6.11.2 mingw_64 目录'
$CompilerBin = Read-Host 'MinGW 13.1.0 的 bin 目录'
$CMakeBin = Read-Host 'CMake 3.30.5 的 bin 目录'
$NinjaBin = Read-Host 'Ninja 所在目录'
$BuildDir = Read-Host '纯英文构建目录'
$env:PATH = "$QtPrefix/bin;$CompilerBin;$NinjaBin;" + $env:PATH
& "$CMakeBin/cmake.exe" -S . -B $BuildDir -G Ninja `
    "-DCMAKE_PREFIX_PATH=$QtPrefix" "-DCMAKE_CXX_COMPILER=$CompilerBin/g++.exe" `
    -DCMAKE_BUILD_TYPE=Debug
if ($LASTEXITCODE -ne 0) { throw 'CMake 配置失败' }
& "$CMakeBin/cmake.exe" --build $BuildDir --parallel 4
if ($LASTEXITCODE -ne 0) { throw '构建失败' }
& "$CMakeBin/ctest.exe" --test-dir $BuildDir --verbose --output-on-failure
if ($LASTEXITCODE -ne 0) { throw '测试失败' }
& "$BuildDir/QtEnvCheck.exe"
```

## 每项 PASS 的含义

| 检查 | 实际验证 |
|---|---|
| Widgets | 编译与运行 Qt 均为 6.11.2，窗口有原生句柄并成功显示到桌面 |
| Svg | 从 qrc 加载真实 SVG，用 QSvgRenderer 渲染到 QImage，校验绿色圆形和透明角落，窗口显示该图像 |
| Sql / SQLite | QSQLITE 驱动、创建内存库、建表、参数化写入、按条件查询、中文字段回读和行数，结束后释放连接 |
| Multimedia | QMediaPlayer 后端可用，QMediaPlayer 与 QAudioOutput 创建、关联和解除关联；本项未执行真实音频播放 |
| Test | 用 Qt Test 真正执行文本断言、QSignalSpy 信号捕获及参数断言，返回值决定 PASS / FAIL |

`QtEnvCheck.Unit` 另含版本、窗口生命周期、SVG 像素、缺失 SVG 失败、SQLite 重复读写与连接清理、缺失驱动失败、多媒体对象关联测试。负向用例的 PASS 表示成功捕获预期失败。

## 文件

- `src/`、`tests/`、`assets/`、`resources.qrc`：完整源码与资源。
- `TEST_REPORT_TEMPLATE.md`：四人成员报告模板。
- `evidence/VALIDATION.md`：本次真实验证结果及日志说明。
- `evidence/*.log`：实际配置、构建及测试日志；机器路径已替换为变量占位符。

提交时保留源码、模板和日志；`.gitignore` 自动排除 `build*`、本机 Creator 配置及本机 CMake 预设。
