#pragma once
#include <QtWidgets>
#include <functional>
#include "services/AppServices.h"
namespace fg {
enum class ButtonRole { Primary, Outline, Danger, Soft, Yellow, Quiet, Icon };
QFrame* createCard(const QString& componentId,QWidget* parent=nullptr);
QPushButton* createButton(const QString& componentId,const QString& text,ButtonRole role,QWidget* parent=nullptr);
void setSmallButton(QPushButton& button);
QString objectId(QString id);
class TimerRing : public QWidget {
 Q_OBJECT
public: explicit TimerRing(QWidget* p=nullptr):QWidget(p){} void setSnapshot(const FocusSnapshot& snapshot);
protected: void paintEvent(QPaintEvent*)override;
private: FocusSnapshot snapshot_;
};
class WeekBarChart : public QWidget {
 Q_OBJECT
public: explicit WeekBarChart(QWidget* p=nullptr):QWidget(p){} void setDays(const QVector<DailyFocus>& days);
protected: void paintEvent(QPaintEvent*)override;
private: QVector<DailyFocus> days_;
};
class StateView : public QWidget {
 Q_OBJECT
public: explicit StateView(QWidget* p=nullptr); void setState(const QString& assetPath,const QString& message);
private: QLabel *art_,*text_;
};
class Sidebar : public QFrame {
 Q_OBJECT
public: explicit Sidebar(QWidget* p=nullptr);void select(PageId id);
signals: void pageRequested(fg::PageId page);
private: QVector<QPushButton*> buttons_;
};
class TopHeader : public QFrame { public: explicit TopHeader(QWidget* p=nullptr); };
// Builds the frozen JSON layouts once. Page owners connect business behavior in their own files.
class LayoutPage : public QWidget {
 Q_OBJECT
public:
 LayoutPage(AppServices& services,const QString& key,QWidget* p=nullptr);
 void refreshView();
 QWidget* component(const QString& id)const;
signals: void actionRequested(QString action);
protected:
 AppServices& services_;QString key_;
 void showResult(const OperationResult& result);
private:
 QWidget* build(const QJsonObject& node);
 QHash<QString,QWidget*> nodes_;QHash<QString,QString> bindings_;
};
}
