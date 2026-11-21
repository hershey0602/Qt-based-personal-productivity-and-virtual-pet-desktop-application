# Qt 信号槽唯一连接表

FG-API-1.0。接口signals声明以API_CONTRACT为准；本表补齐A门面和UI槽。表中void是声明的一部分，connect时不写void；成员函数指针连接必须使用对应声明类型。连接只在AppComposition/MainWindow构造时建立一次。

| 发出方 | 准确信号声明 | 接收方 | 准确槽或可连接方法 | 触发时机 | 连接与副作用 |
|---|---|---|---|---|---|
| TaskManager | void tasksChanged(); | AppServices | void onTasksChanged(); | 成功CRUD后一次 | Auto；门面汇总dataChanged |
| SettingsService | void preferencesChanged(fg::Preferences preferences); | FocusEngine | void onPreferencesChanged(fg::Preferences preferences); | sync成功后；非成功不发 | Auto，同线程；仅Idle默认草稿可更新 |
| SettingsService | void preferencesChanged(fg::Preferences preferences); | AppServices | void onPreferencesChanged(fg::Preferences preferences); | 同上 | 门面转发同名信号给设置页与A |
| FocusEngine | void focusChanged(fg::FocusSnapshot snapshot); | AppServices | void onFocusChanged(fg::FocusSnapshot snapshot); | tick/状态变更，最多150ms刷新 | 门面转发；不重建页面 |
| FocusEngine | void completionPersisted(fg::SessionId sessionId); | CompletionCoordinator | void settle(fg::SessionId sessionId); | B completeSession提交成功后 | Auto；同步调C，只接一次 |
| FocusEngine | void completionPersisted(fg::SessionId sessionId); | CompletionSound | void playForCompletedSession(const fg::SessionId& sessionId); | 同上；非休息自然完成 | Auto；D读取音效开关并按ID去重 |
| CompletionCoordinator | void settlementCompleted(fg::SettlementReceipt receipt); | AppServices | void onSettlementCompleted(fg::SettlementReceipt receipt); | C返回ok后 | Coordinator先调用B applySettlementSuccess再发本信号；门面只dataChanged/新到账focusCompleted，不再apply |
| CompletionCoordinator | void settlementFailed(fg::SessionId sessionId, fg::AppError error); | AppServices | void onSettlementFailed(fg::SessionId sessionId, fg::AppError error); | C返回失败后 | Coordinator先调用B applySettlementFailure再发本信号；门面只一条错误notice |
| PetManager | void petChanged(fg::PetSnapshot pet); | AppServices | void onPetChanged(fg::PetSnapshot pet); | 喂食提交成功；重复操作不改值 | 汇总dataChanged |
| PetManager | void petLeveled(fg::FeedReceipt receipt); | AppServices | void onPetLeveled(fg::FeedReceipt receipt); | newlyApplied且levelAfter>Before | 门面转发给A结果框 |
| MusicController | void musicChanged(fg::MusicSnapshot snapshot); | AppServices | void onMusicChanged(fg::MusicSnapshot snapshot); | 后端播放/进度/清单更新 | 转MusicPage，只读渲染 |
| AppServices | void dataChanged(); | MainWindow | void refreshAll(); | 成功业务写入后 | QueuedConnection，保护正在点击的行 |
| AppServices | void focusChanged(fg::FocusSnapshot snapshot); | FocusPage | void refreshTimerOnly(fg::FocusSnapshot snapshot); | 计时tick/状态变更 | Auto，仅更新时间环和状态按钮 |
| AppServices | void preferencesChanged(fg::Preferences preferences); | SettingsPage | void renderPreferences(fg::Preferences preferences); | D保存成功后 | QSignalBlocker避免渲染反向写配置 |
| AppServices | void musicChanged(fg::MusicSnapshot snapshot); | MusicPage | void renderMusic(fg::MusicSnapshot snapshot); | 后台真实状态变更 | Auto；离页播放器照常运行 |
| AppServices | void focusCompleted(fg::CompletionResult result); | MainWindow | void onFocusCompleted(fg::CompletionResult result); | newlyCredited成功收入 | 当前Focus结果框，否则轻提示；不在槽内再发币 |
| AppServices | void petLeveled(fg::FeedReceipt receipt); | MainWindow | void onPetLeveled(fg::FeedReceipt receipt); | 真实新升级 | ResultDialog静态升级 |
| AppServices | void notice(QString message, fg::NoticeLevel level); | MainWindow | void showNotice(QString message, fg::NoticeLevel level); | 异步错误/结果说明 | GUI线程单一提示出口 |
| Sidebar | void pageRequested(fg::PageId page); | MainWindow | void navigate(fg::PageId page); | 用户点导航 | 只setCurrentWidget |
| DashboardPage/TaskPage | void newTaskRequested(); | MainWindow | void openNewTaskDialog(); | 用户新建 | 同TaskEditDialog(new) |
| TaskPage | void editTaskRequested(fg::TaskId taskId); | MainWindow | void openEditTaskDialog(fg::TaskId taskId); | 用户编辑 | 同TaskEditDialog(edit) |
| TaskPage | void deleteTaskRequested(fg::TaskId taskId); | MainWindow | void confirmDeleteTask(fg::TaskId taskId); | 用户删除 | ConfirmDialog提交回调成功才accept |
| TaskPage | void focusTaskRequested(fg::TaskId taskId); | MainWindow | void openFocusForTask(fg::TaskId taskId); | 未完成任务专注入口 | 先导航；preselectFocusTask，不start |
| DashboardPage | void focusPresetRequested(int minutes); | MainWindow | void openFocusPreset(int minutes); | 快捷25/50 | 活动冲突只显示原会话 |
| DashboardPage/FocusPage | void statisticsRequested(); | MainWindow | void showStatisticsHistory(); | 完整统计/历史入口 | navigate Statistics + revealHistory |
| TaskEditDialog | void taskSaved(fg::TaskId taskId); | MainWindow | void selectSavedTask(fg::TaskId taskId); | 保存成功后、accept前 | 只更新选择；数据刷新已由dataChanged负责 |

| FocusEngine | void restFinished(); | AppServices | void onRestFinished(); | 休息自然结束并已回Idle | 发notice普通休息完成提示，不发币、不写有效记录 |
| DashboardPage | void navigationRequested(fg::PageId page); | MainWindow | void navigate(fg::PageId page); | 去任务/宠物/音乐等只跳页入口 | 只导航，不暗中开始/喂食/播放 |
| TaskManager/SettingsService/FocusEngine/RewardService/PetManager/MusicController/CompletionSound | void errorOccurred(fg::AppError error); | AppServices | void onServiceError(fg::AppError error); | 仅异步后台错误 | 各源连接一次；转单一notice；同步返回失败不再发此信号 |

## 接口声明与连接细节

表中的AppServices onTasksChanged/onPreferencesChanged/onFocusChanged/onSettlementCompleted/onSettlementFailed/onPetChanged/onPetLeveled/onMusicChanged/onRestFinished/onServiceError由A实现为private slots，参数照表逐字生成。MainWindow/页面槽由该类负责人实现。CompletionSound::playForCompletedSession在接口中是public普通方法，函数指针connect同样可连接，不要求把它再声明一次slot。

RewardService::rewardCommitted仅供记录/独立测试；AppServices不同时订阅它和Coordinator成功信号来发两次到账。唯一页面结算通知由Coordinator结果驱动。所有服务errorOccurred(fg::AppError error)仅用于异步/后台错误，A连接到门面统一notice；同步方法失败由调用页面处理返回值，不再为同一失败发errorOccurred，防止重复弹窗。

同步状态渲染采用QSignalBlocker，不让setCurrentIndex/toggled把程序刷新当作用户输入。时钟tick不发dataChanged；刷新不执行SQL写入。数据刷新和媒体进度都在GUI线程；本项目不额外启用后台数据库线程，不能跨线程共享一个QSqlDatabase。

QMetaType注册详见DATA_MODELS。页构造与公共组件方法见UI_BINDING_CONTRACT。本表新增/修改必须和API注册表同一PR，A合并，B/C/D按新版本更新。
