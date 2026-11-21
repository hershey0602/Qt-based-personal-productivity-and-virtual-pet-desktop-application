# 页面接入与公共组件声明

FG-API-1.0。A第1天先生成下列声明与占位，之后各成员在同一文件补实现。本文件只规定代码生成目标，不交付这些C++文件。

```cpp
// 每页各自类声明，类名与负责人如下；参数均为此签名
explicit DashboardPage(fg::AppServices& services, QWidget* parent=nullptr); // A
explicit TaskPage(fg::AppServices& services, QWidget* parent=nullptr);      // B
explicit FocusPage(fg::AppServices& services, QWidget* parent=nullptr);     // B
explicit PetPage(fg::AppServices& services, QWidget* parent=nullptr);       // C
explicit StatisticsPage(fg::AppServices& services, QWidget* parent=nullptr);// B
explicit MusicPage(fg::AppServices& services, QWidget* parent=nullptr);     // D
explicit SettingsPage(fg::AppServices& services, QWidget* parent=nullptr);  // D
// 七页都提供public slots: void refresh();
// 额外可连接方法：
void FocusPage::refreshTimerOnly(fg::FocusSnapshot snapshot);
void StatisticsPage::revealHistory();
void TaskPage::selectTask(fg::TaskId taskId); // 保存后更新选择，查不到/已软删则清空
void MusicPage::renderMusic(fg::MusicSnapshot snapshot);
void SettingsPage::renderPreferences(fg::Preferences preferences);

TaskEditDialog(fg::AppServices& services, std::optional<fg::TaskId> taskId,
               QWidget* parent=nullptr); // B，新增显式传std::nullopt
ConfirmDialog(const QString& title, const QString& body,
              const QString& confirmText, QWidget* parent=nullptr); // A
void ConfirmDialog::setCommitAction(std::function<fg::OperationResult()> action);
ResultDialog(const fg::CompletionResult& result, QWidget* parent=nullptr); // A
ResultDialog(const fg::FeedReceipt& result, QWidget* parent=nullptr);      // A 升级重载
```

ConfirmDialog确认按钮先执行commit action；失败留窗并显示error.message，成功才accept。TaskEditDialog保存调用saveTask(draft,id)，成功发taskSaved；取消/X/Esc只有未改动可直接reject，否则二层确认继续编辑保留输入。

## 公共组件唯一入口

```cpp
namespace fg {
enum class ButtonRole { Primary, Outline, Danger, Soft, Yellow, Quiet, Icon };
QFrame* createCard(const QString& componentId, QWidget* parent=nullptr);
QPushButton* createButton(const QString& componentId, const QString& text,
                         ButtonRole role, QWidget* parent=nullptr);
void setSmallButton(QPushButton& button);  // size=small，30高、12px、radius9
void TimerRing::setSnapshot(const FocusSnapshot& snapshot);
void WeekBarChart::setDays(const QVector<DailyFocus>& days);
void StateView::setState(const QString& assetPath, const QString& message);
}
```

这些声明生成在src/ui/Components.h；A提供最小可用实现，成员复用，不重定义控件类或各写一份QSS。componentId点换下划线为ObjectName，role/kind/selected等动态属性保持现有QSS契约。布局仍以UI V2.1组件基线为准。

## 唯一页面注册

MainWindow构造一次new七页，按PageId固定顺序addWidget；setCurrentWidget只切换。page.refresh读取门面；页面对象不持有服务所有权，不new服务。TaskPage保存filter、selectedTaskId、滚动位置，refresh不清空筛选。MainWindow::refreshAll调用七页refresh，采用排队连接；Focus tick只调refreshTimerOnly。

UI参数表旧“业务接口”列及HTML action字符串是交互标签，不是可复制的C++签名。最新参数/返回值以API_CONTRACT和本文件为准；例如旧configureFocus(taskId,minutes,mode)改用FocusConfig，旧abandonFocus()必须带expectedSessionId。控件ID、视觉参数、QRC路径不改。

## A的应用装配入口

```cpp
AppComposition(const QString& appDataDirectory, fg::ModelMode mode,
               QObject* parent=nullptr);
fg::OperationResult AppComposition::initialize();
fg::AppServices& AppComposition::services() const; // initialize成功后才调用
void AppComposition::shutdown();                 // 活动专注已处理后释放服务/连接
MainWindow(fg::AppServices& services, QWidget* parent=nullptr);
```

AppComposition在main创建一次并以QApplication为parent，MainWindow保存services非拥有引用。initialize失败显示错误，不构造伪成功Live窗口。main统一设置组织名FocusGardenStudentTeam和应用名FocusGarden，D设置文件显式用传入AppData目录/app.ini；B库同目录/focusgarden.sqlite。

## UI信号与公共接收方法

Sidebar::pageRequested、DashboardPage::navigationRequested、DashboardPage::focusPresetRequested/newTaskRequested/statisticsRequested、TaskPage::newTaskRequested/editTaskRequested/deleteTaskRequested/focusTaskRequested、FocusPage::statisticsRequested及TaskEditDialog::taskSaved的参数逐字见SIGNAL_SLOT_MATRIX；各页负责人只在本类声明一次。MainWindow::selectSavedTask调用同一个TaskPage::selectTask，不重建任务页。全部MainWindow槽及AppServices内部转发槽也以该表的完整签名生成。

AppServices::onRestFinished()只发notice(QString,NoticeLevel::Info)显示普通休息结束，不能走专注金币结果框。AppServices::onServiceError(fg::AppError)只处理异步错误；同步命令由调用者依据返回值显示一次失败。
