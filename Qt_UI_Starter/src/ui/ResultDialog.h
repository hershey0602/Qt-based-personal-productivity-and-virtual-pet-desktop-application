#pragma once
#include "Components.h"
namespace fg { class ResultDialog : public QDialog {
public: ResultDialog(const fg::CompletionResult& result,QWidget* parent=nullptr);
ResultDialog(const fg::FeedReceipt& result,QWidget* parent=nullptr);
private: void content(const QString& title,const QString& text);
}; }
