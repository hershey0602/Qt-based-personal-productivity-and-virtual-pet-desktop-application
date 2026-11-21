#include <QtTest>
#include <QSvgRenderer>
#include "AppComposition.h"
#include "MainWindow.h"
#include "mocks/MockPorts.h"
#include "ui/ConfirmDialog.h"
using namespace fg;
class ShellTest : public QObject {
 Q_OBJECT
private slots:
 void navigation(){AppComposition c({},ModelMode::Demo);QVERIFY(c.initialize().ok);MainWindow w(c.services());w.show();auto stack=w.findChild<QStackedWidget*>("main_pages");QCOMPARE(stack->count(),7);auto originals=w.findChildren<LayoutPage*>();QStringList keys={"dashboard","tasks","focus","pet","statistics","music","settings"};for(int round=0;round<3;++round)for(int i=0;i<7;++i){auto b=w.findChild<QPushButton*>("nav_"+keys[i]);QVERIFY(b);QTest::mouseClick(b,Qt::LeftButton);QCOMPARE(stack->currentIndex(),i);}auto after=w.findChildren<LayoutPage*>();QCOMPARE(after.size(),originals.size());for(auto page:originals)QVERIFY(after.contains(page));}
 void resources(){QDirIterator it(":/fg",QDir::Files,QDirIterator::Subdirectories);int count=0;while(it.hasNext()){auto path=it.next();QFile f(path);QVERIFY2(f.open(QIODevice::ReadOnly),qPrintable(path));QVERIFY(f.size()>0);if(path.endsWith(".svg")){QSvgRenderer r(path);QVERIFY2(r.isValid(),qPrintable(path));}if(path.endsWith(".png"))QVERIFY(!QImage(path).isNull());++count;}QCOMPARE(count,45);}
 void demoDefaults(){AppComposition c({},ModelMode::Demo);QVERIFY(c.initialize().ok);auto& s=c.services();QVERIFY(s.listTasks({}).value.isEmpty());QCOMPARE(s.pet().value.coins,0);QCOMPARE(s.pet().value.mood,70);QCOMPARE(s.statistics().value.last7Days.size(),7);QVERIFY(!s.startFocus().ok);QCOMPARE(s.feedPet().error.code,AppErrorCode::NotImplemented);QVERIFY(!s.saveTask({},std::nullopt).ok);QVERIFY(!s.savePreferences({}).ok);}
 void presetDoesNotStart(){AppComposition c({},ModelMode::Demo);QVERIFY(c.initialize().ok);MainWindow w(c.services());w.openFocusPreset(50);QCOMPARE(c.services().focus().state,FocusState::Idle);QCOMPARE(c.services().focus().remainingSeconds,3000);w.navigate(PageId::Music);w.navigate(PageId::Focus);QCOMPARE(c.services().focus().remainingSeconds,3000);}
 void failedCommitKeepsDialog(){ConfirmDialog d("title","body","save");d.setCommitAction([]{return OperationResult{false,{AppErrorCode::Persistence,"保存失败",{}}};});d.show();QTest::mouseClick(d.findChild<QPushButton*>("confirm_submit"),Qt::LeftButton);QVERIFY(d.isVisible());QCOMPARE(d.result(),0);}
 void liveRequiresImplementation(){AppComposition c({},ModelMode::Live);QVERIFY(!c.initialize().ok);}
};
QTEST_MAIN(ShellTest)
#include "test_shell.moc"

