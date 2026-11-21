#include "PetPage.h"
namespace fg {
PetPage::PetPage(AppServices& services,QWidget* parent):LayoutPage(services,"pet",parent){
connect(this,&LayoutPage::actionRequested,this,[this](QString a){if(a=="pet.feed"){auto r=services_.feedPet();showResult({r.ok,r.error});}});
refresh();
}
void PetPage::refresh(){refreshView();}
}
