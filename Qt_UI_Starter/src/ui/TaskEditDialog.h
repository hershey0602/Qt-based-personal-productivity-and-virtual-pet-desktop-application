#pragma once
#include "ConfirmDialog.h"
namespace fg { class TaskEditDialog : public QDialog {
 Q_OBJECT
public: TaskEditDialog(fg::AppServices& services,std::optional<fg::TaskId> taskId,QWidget* parent=nullptr);
public slots: void reject()override;
signals: void taskSaved(fg::TaskId taskId);
private: AppServices& services_;std::optional<TaskId> id_;QLineEdit* title_;QTextEdit* note_;QComboBox *category_,*priority_;QCheckBox* hasDue_;QDateTimeEdit* due_;QLabel* error_;QString initial_;
 QString state()const;
}; }
