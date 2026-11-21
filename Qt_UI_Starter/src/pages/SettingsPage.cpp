#include "SettingsPage.h"
namespace fg {
SettingsPage::SettingsPage(AppServices& services,QWidget* parent):LayoutPage(services,"settings",parent){
auto save=[this]{Preferences p;p.defaultFocusMinutes=qobject_cast<QComboBox*>(component("settings.focusMinutes"))->currentIndex()?50:25;p.defaultRestMinutes=qobject_cast<QComboBox*>(component("settings.restMinutes"))->currentIndex()?10:5;p.completionSoundEnabled=qobject_cast<QCheckBox*>(component("settings.sound"))->isChecked();auto r=services_.savePreferences(p);if(!r.ok){renderPreferences(services_.preferences());showResult(r);}};
 connect(qobject_cast<QComboBox*>(component("settings.focusMinutes")),&QComboBox::currentIndexChanged,this,[save](int){save();});
 connect(qobject_cast<QComboBox*>(component("settings.restMinutes")),&QComboBox::currentIndexChanged,this,[save](int){save();});
 connect(qobject_cast<QCheckBox*>(component("settings.sound")),&QCheckBox::toggled,this,[save](bool){save();});
refresh();
}
void SettingsPage::refresh(){refreshView();renderPreferences(services_.preferences());}
void SettingsPage::renderPreferences(fg::Preferences p){auto f=qobject_cast<QComboBox*>(component("settings.focusMinutes"));auto r=qobject_cast<QComboBox*>(component("settings.restMinutes"));auto s=qobject_cast<QCheckBox*>(component("settings.sound"));QSignalBlocker a(f),b(r),c(s);f->setCurrentIndex(p.defaultFocusMinutes==50);r->setCurrentIndex(p.defaultRestMinutes==10);s->setChecked(p.completionSoundEnabled);}
}
