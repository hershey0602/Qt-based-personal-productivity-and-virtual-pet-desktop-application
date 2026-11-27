# D 验收记录（D-T01 ～ D-T12）

验收日期：2026-11-27
范围说明：UI 级验收依赖 A 的 Live 装配（`--live` 当前返回 NotImplemented），因此本轮证据以**服务级真实运行**为主：真实 `QMediaPlayer` 播放真实 wav、真实 `QSettings` 读写临时 ini。凡未覆盖的项一律写“未执行/部分”，不以上述结果冒充 Live。

## 环境

| 项 | 值 |
|---|---|
| 编译器 | `g++.exe (x86_64-posix-seh-rev1, Built by MinGW-Builds project) 13.1.0` |
| CMake | 3.30（`C:\Qt\Tools\CMake_64\bin\cmake.exe`） |
| Qt | 6.11.2 mingw_64 |
| 生成器 | MinGW Makefiles |
| 分支 | `feature/D-music-settings` |
| 提交 SHA | `6115291`（feat(D): 实现音乐播放、设置持久化与完成音效） |

## 逐项结果

| 编号 | 结果 | 证据 | 说明 |
|---|---|---|---|
| D-T01 | **通过（服务级）** | `logs/d-verify-run.txt` → `d_t01_loadAndPlayWav` | 加载 2 首 wav，`select` 后 `playPause` 进入 Playing，position 推进，duration>0；导入后不自播 |
| D-T02 | **部分** | `d_t02_pauseAndStop` | 暂停冻结、停止归零已通过；**跨任务/宠物页回来继续播未执行**（依赖 A 的页面导航与 Live 装配） |
| D-T03 | **通过（服务级）** | `d_t03_previousNextWrap` | 末曲 next 回首曲、首曲 previous 到末曲 |
| D-T04 | **部分** | `d_t04_seekAndVolume` | seek 到中点误差<150ms、音量 0/100 生效；**不可 seek 媒体的错误返回未单独覆盖** |
| D-T05 | **部分** | `d_t05_badPathSingleError` | 坏文件只报一条错误且不崩（1 次）；**移除当前项/非当前项、播放中移动或删除磁盘文件未覆盖** |
| D-T06 | **通过（服务级）** | `d_t06_rebuildRestoresWithoutAutoplay` | 重建实例后歌单与音量恢复，state=Stopped，currentIndex 为空 |
| D-T07 | **通过（服务级）** | `d_t07_preferencesPersist` | 25→50、5→10、关音效写盘后读回一致；**设置页即时显示一致未执行** |
| D-T08 | **未执行** | — | 依赖 B 的 FocusEngine 长跑后 prepareNextRound |
| D-T09 | **部分** | `d_t09_completionSoundSwitchAndDedup` | 同 sessionId 不重复播放、开关关闭不播；**注入设置写入失败回旧值未覆盖** |
| D-T10 | **未执行** | UI 截图 `D_UI_settings_default.png`（仅证明页面可打开） | 需要在 Live 装配下核对“只有三项可改”并与服务一致 |
| D-T11 | **未执行** | — | 日 5 全组功能清单 |
| D-T12 | **未执行** | — | 需另一台未装 Qt 的 Windows 电脑 |

汇总：通过 5 项（D-T01/03/06/07 服务级 + 构建链），部分 4 项（D-T02/04/05/09），未执行 4 项（D-T08/10/11/12）。

## 构建与测试

| 步骤 | 命令要点 | 退出码 | 日志 |
|---|---|---|---|
| 首次配置（失败留证） | 未指定编译器，CMake 未自动发现 GCC | 1 | `logs/d-verify-configure.log` |
| 补充编译器后配置 | `-DCMAKE_CXX_COMPILER=.../g++.exe` | 0 | `logs/d-verify-configure-with-g++.log` |
| 构建（历史失败） | 源码清单包含他人页面/服务，链接期 19 个未定义符号 | 2 | `logs/d-verify-build.log` |
| 构建（成功） | 仅编 D 三个服务 + `ServicePorts.h`(moc) + `resources.qrc` | 0 | `logs/d-verify-build-success.log` |
| 运行测试 | `d_service_verify.exe -o <txt>,txt` | 0 | `logs/d-verify-run.txt`、`logs/d-verify-run-stderr.log` |

测试汇总：`Totals: 10 passed, 0 failed, 0 skipped, 0 blacklisted, 2965ms`（耗时说明真实解码与播放，非空跑）。

## 运行证据

| 文件 | 内容 |
|---|---|
| `D_UI_live_blocked.png` | `FocusGarden.exe --live` 弹“当前为第1天公共框架，请以演示模式运行” |
| `D_UI_demo_mock.png` | Demo 模式点“添加本地音乐”弹“演示模式：此业务由对应成员接入”，歌单为空 |
| `D_UI_settings_default.png` | 设置页默认值（25/50、5/10） |
| `logs/d-verify-run.txt` | 10 条用例逐条 PASS |
| `logs/d-verify-run-stderr.log` | ffmpeg 解码 wav 的实际输出（22050 Hz, pcm_s16le） |

缺口：缺 1280/1600/1920 三种窗口尺寸的页面截图；缺另一台电脑的运行证据。
