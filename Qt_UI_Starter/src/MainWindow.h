#pragma once
#include "ui/Components.h"
namespace fg { class MainWindow : public QMainWindow {
 Q_OBJECT
public: explicit MainWindow(fg::AppServices& services,QWidget* parent=nullptr);
public slots:
void navigate(fg::PageId page);void refreshAll();void openNewTaskDialog();void openEditTaskDialog(fg::TaskId taskId);void confirmDeleteTask(fg::TaskId taskId);void openFocusForTask(fg::TaskId taskId);void openFocusPreset(int minutes);void showStatisticsHistory();void selectSavedTask(fg::TaskId taskId);void onFocusCompleted(fg::CompletionResult result);void onPetLeveled(fg::FeedReceipt receipt);void showNotice(QString message,fg::NoticeLevel level);
protected: void closeEvent(QCloseEvent* event)override;
private: AppServices& services_;QStackedWidget* stack_;Sidebar* sidebar_;QLabel* notice_;QVector<LayoutPage*> pages_;
}; }
