# 本次实际验证记录

日期：2026-11-22（北京时间）。

| 项目 | 实测结果 |
|---|---|
| 操作系统 | Windows 11 x64，内核 10.0.26200.0 |
| Qt 编译 / 运行版本 | 6.11.2 / 6.11.2 |
| 编译器 | MinGW GCC 13.1.0，x86_64-posix-seh |
| CMake / C++ | 3.30.5 / C++17（201703） |
| 构建 | Debug，Ninja，全部目标编译链接成功 |
| QtEnvCheck.Unit | PASS |
| QtEnvCheck.Window | PASS；实际桌面窗口自检，五项均 PASS |
| 单独导出 Qt Test 详细日志 | 9 passed，0 failed，0 skipped（含初始化和清理） |
| 人工读图核对 | 编译及运行版本、绿色 SVG 勾号、五行 PASS 和底部 PASS：5/5 均清晰显示，见 window.png |

Multimedia 的 PASS 对应后端可用、QMediaPlayer / QAudioOutput 对象创建、关联和解除关联，本次未执行真实音频播放。

## 实际日志

- `toolchain.log`：工具版本与系统版本。
- `configure.log`：最终配置输出，环境精确版本校验通过。
- `build.log`：最终完整构建输出，14 个构建步骤成功。
- `ctest.log`、`LastTest.log`：CTest 两组测试通过及桌面窗口五项 PASS 输出。
- `qttest.log`：Qt Test 每个真实用例的详细结果。`missingSvgFails` 的“文件不存在”警告是负向测试的预期输出。
- `build-initial.log`：首次中文构建目录下的 moc 写入失败记录。最终使用沙箱临时纯英文构建目录，完成配置、构建、CTest 后复制可执行文件供窗口读图核对。
- `window.png`：本次实际程序窗口截图。

配置输出中未找到 Vulkan headers 的提示未阻止本工程五模块配置和构建；沙箱临时目录含 `#` 导致 Qt Test 辅助构建目录宏被 CMake 省略，本工程用 qrc 访问测试资源，最终用例全部通过。日志保留这些真实输出。

日志仅将机器目录替换为 `<QT_PREFIX>`、`<MINGW_BIN>`、`<CMAKE_BIN>`、`<WORKSPACE>` 和 `<BUILD_DIR>`，其余输出保持实际执行内容。源代码及 CMake 均无个人安装路径。

## 本次执行命令（路径以变量表示）

```powershell
& "$QtPrefix/bin/qmake.exe" -query QT_VERSION
& "$CompilerBin/g++.exe" --version
& "$CMakeBin/cmake.exe" --version
$env:PATH = "$QtPrefix/bin;$CompilerBin;$NinjaBin;" + $env:PATH
& "$CMakeBin/cmake.exe" -S tools/QtEnvCheck -B $BuildDir -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_PREFIX_PATH=$QtPrefix" `
    "-DCMAKE_CXX_COMPILER=$CompilerBin/g++.exe"
& "$CMakeBin/cmake.exe" --build $BuildDir --parallel 4
& "$CMakeBin/ctest.exe" --test-dir $BuildDir --verbose --output-on-failure
# 将已构建的可执行文件复制到本机 build-verified 后，另导出逐用例日志：
& ./tools/QtEnvCheck/build-verified/QtEnvCheckTests.exe `
    -o tools/QtEnvCheck/evidence/qttest.raw.log,txt
& ./tools/QtEnvCheck/build-verified/QtEnvCheck.exe
```
