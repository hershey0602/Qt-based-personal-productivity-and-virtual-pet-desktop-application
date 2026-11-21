#include <QtWidgets>
#include "AppComposition.h"
#include "MainWindow.h"
int main(int argc,char** argv){
 QApplication app(argc,argv);app.setOrganizationName("FocusGardenStudentTeam");app.setApplicationName("FocusGarden");app.setApplicationVersion("0.1.0");fg::registerMetaTypes();
 QFont font("Microsoft YaHei");font.setPixelSize(14);app.setFont(font);QFile qss(":/fg/main.qss");if(qss.open(QIODevice::ReadOnly))app.setStyleSheet(QString::fromUtf8(qss.readAll()));
 auto composition=new fg::AppComposition(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),app.arguments().contains("--live")?fg::ModelMode::Live:fg::ModelMode::Demo,&app);auto initialized=composition->initialize();if(!initialized.ok){QMessageBox::critical(nullptr,QStringLiteral("拾光岛"),initialized.error.message);return 1;}
 fg::MainWindow window(composition->services());window.show();
 int capture=app.arguments().indexOf("--capture");
 if(capture>=0&&capture+1<app.arguments().size()){
  QString directory=app.arguments()[capture+1];QDir().mkpath(directory);
  QTimer::singleShot(250,&window,[&window,directory,&app]{QStringList keys={"dashboard","tasks","focus","pet","statistics","music","settings"};for(int i=0;i<7;++i){window.navigate(fg::PageId(i));app.processEvents();window.grab().save(directory+"/"+keys[i]+".png");}for(auto size:{QSize(1280,720),QSize(1366,768),QSize(1600,900)}){window.resize(size);window.navigate(fg::PageId::Dashboard);app.processEvents();window.grab().save(directory+QString("/dashboard_%1x%2.png").arg(size.width()).arg(size.height()));}app.exit(0);});
 }
 int code=app.exec();composition->shutdown();return code;
}
