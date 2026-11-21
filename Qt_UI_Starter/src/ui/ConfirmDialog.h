#pragma once
#include "Components.h"
namespace fg { class ConfirmDialog : public QDialog {
 Q_OBJECT
public: ConfirmDialog(const QString& title,const QString& body,const QString& confirmText,QWidget* parent=nullptr);
 void setCommitAction(std::function<fg::OperationResult()> action);
private: std::function<OperationResult()> action_;QLabel* error_;
}; }
