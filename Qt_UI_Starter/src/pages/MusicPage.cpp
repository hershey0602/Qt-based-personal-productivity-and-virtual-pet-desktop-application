#include "MusicPage.h"
namespace fg {
MusicPage::MusicPage(AppServices& services,QWidget* parent):LayoutPage(services,"music",parent){
connect(this,&LayoutPage::actionRequested,this,[this](QString a){if(a=="music.import"){auto paths=QFileDialog::getOpenFileNames(this,QStringLiteral("添加本地音乐"),{},QStringLiteral("音频 (*.mp3 *.wav *.flac *.ogg *.m4a)"));if(!paths.isEmpty())showResult(services_.music().importFiles(paths));}});
refresh();
}
void MusicPage::refresh(){refreshView();renderMusic(services_.music().snapshot());}
void MusicPage::renderMusic(fg::MusicSnapshot s){for(auto button:findChildren<QPushButton*>()){auto id=button->objectName();if(id=="music_play"||id=="music_stop"||id=="music_previous"||id=="music_next"||id=="music_remove")button->setEnabled(!s.tracks.isEmpty());}QSignalBlocker b(component("music.volumeSlider"));qobject_cast<QSlider*>(component("music.volumeSlider"))->setValue(s.volume);}
}
