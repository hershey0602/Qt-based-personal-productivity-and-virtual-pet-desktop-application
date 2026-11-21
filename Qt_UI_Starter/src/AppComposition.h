#pragma once
#include "services/AppServices.h"
namespace fg { class AppComposition : public QObject {
 Q_OBJECT
public: AppComposition(const QString& appDataDirectory,fg::ModelMode mode,QObject* parent=nullptr);
 fg::OperationResult initialize();fg::AppServices& services()const;void shutdown();
private: QString directory_;ModelMode mode_;AppServices* services_=nullptr;
}; }
