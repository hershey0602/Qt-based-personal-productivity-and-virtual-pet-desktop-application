# D 业务与接口清单

按 FG-API-1.0，D 负责三个服务端口与两个页面。接口声明在 `Qt_UI_Starter/src/contracts/ServicePorts.h`，实现由 D 提交。

## 一、端口方法

### ISettingsService → `SettingsService`
构造：`SettingsService(const QString& settingsFilePath, QObject* parent = nullptr)`

| 方法 | 说明 |
|---|---|
| `OperationResult load()` | 读取 ini；校验失败返回 Validation 并保留默认值 |
| `Preferences preferences() const` | 当前内存中的偏好 |
| `OperationResult savePreferences(const Preferences&)` | 校验后落盘 |
| `MusicPreferences musicPreferences() const` | 当前内存中的音乐偏好 |
| `OperationResult saveMusicPreferences(const MusicPreferences&)` | 校验后落盘 |

信号：`preferencesChanged(fg::Preferences)`、`musicPreferencesChanged(fg::MusicPreferences)`、`errorOccurred(fg::AppError)`

### IMusicController → `MusicController`
构造：`MusicController(ISettingsService& settings, QObject* parent = nullptr)`

| 方法 | 说明 |
|---|---|
| `OperationResult loadPlaylist()` | 从设置恢复歌单，不自动播放 |
| `MusicSnapshot snapshot() const` | 当前曲目、索引、播放态、进度、时长、音量、seekable |
| `OperationResult importFiles(const QStringList&)` | 导入绝对路径，去重合并 |
| `OperationResult select(int index)` | 选中，不自播 |
| `OperationResult playPause()` | 播放/暂停切换 |
| `OperationResult stop()` | 停止并归零 |
| `OperationResult previous()` / `next()` | 两端循环 |
| `OperationResult seek(qint64 positionMs)` | 不可 seek 的媒体返回明确错误 |
| `OperationResult setVolume(int volume)` | 0–100 |
| `OperationResult remove(int index)` | 仅从清单移除，不删磁盘文件 |

信号：`musicChanged(fg::MusicSnapshot)`、`errorOccurred(fg::AppError)`

### ICompletionSound → `CompletionSound`
构造：`CompletionSound(ISettingsService& settings, QObject* parent = nullptr)`

| 方法 | 说明 |
|---|---|
| `void playForCompletedSession(const SessionId&)` | 自然完成时播放提示音；同一 sessionId 只播一次；`completionSoundEnabled=false` 时静默返回 |

信号：`errorOccurred(fg::AppError)`

## 二、页面

| 文件 | 绑定行为 |
|---|---|
| `src/pages/MusicPage.cpp/.h` | 导入、选中、双击播放、暂停/停止、上下曲、音量滑杆、移除 |
| `src/pages/SettingsPage.cpp/.h` | 专注 25/50、休息 5/10、完成音效开关三项，改动即保存 |

## 三、持久化与资源路径

存储：`QSettings(settingsPath_, QSettings::IniFormat)`，路径由构造参数传入；生产路径为应用数据目录下的 `app.ini`。

| 键 | 类型 | 默认 | 约束 |
|---|---|---|---|
| `preferences/defaultFocusMinutes` | int | 25 | 仅 25 或 50 |
| `preferences/defaultRestMinutes` | int | 5 | 仅 5 或 10 |
| `preferences/completionSoundEnabled` | bool | true | — |
| `music/localPaths` | QStringList | 空 | 必须是绝对路径 |
| `music/volume` | int | 70 | 0–100 |

写盘语义：`QSettings::status() != NoError` 视为失败，`sync()` 后复查；失败时把旧值写回并返回失败，**不发出成功信号**。

音效资源：`qrc:/fg/UI_ASSETS/audio/sfx_focus_complete.wav`，来自 `FocusGarden/resources.qrc`。

## 四、依赖关系

- `MusicController`、`CompletionSound` 依赖 `ISettingsService`（构造注入），不直接读写 ini
- 三个服务均为 QObject，生命周期由 A 在 `AppComposition` 中持有，MusicPage/SettingsPage 只拿指针
- 与 B 的衔接点：`FocusEngine::completionPersisted(SessionId)` → `CompletionCoordinator` → `ICompletionSound::playForCompletedSession`
