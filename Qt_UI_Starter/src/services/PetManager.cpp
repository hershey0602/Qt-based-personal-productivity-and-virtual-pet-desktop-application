#include "PetManager.h"
#include "mocks/MockPorts.h"
namespace fg {
PetManager::PetManager(IRepository& repository, QObject* parent) : IPetService(parent) {}
Result<PetSnapshot> PetManager::pet() const { return {false, {}, notImplemented()}; }
Result<FeedReceipt> PetManager::feed() { return {false, {}, notImplemented()}; }
}
