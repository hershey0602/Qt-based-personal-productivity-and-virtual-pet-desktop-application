#pragma once
#include "contracts/ServicePorts.h"
namespace fg {
class PetManager : public IPetService {
 Q_OBJECT
public:
 explicit PetManager(IRepository& repository, QObject* parent=nullptr);
Result<PetSnapshot> pet() const override;
Result<FeedReceipt> feed() override;
};
}
