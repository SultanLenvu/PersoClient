#include "async_production_manager.h"
#include "jlink_exe_programmer.h"
#include "named_object_factory.h"
#include "perso_server_connection.h"
#include "te310_printer.h"

AsyncProductionManager::AsyncProductionManager(const QString& name)
    : ProgressableAsyncWrapper(name) {
  createManager();
}

void AsyncProductionManager::logOn(const StringDictionary& param) {
  initOperation("logOn");

  ReturnStatus ret;
  ret = Manager->logOn(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOn", ret);
    return;
  }

  completeOperation("logOn");
}

void AsyncProductionManager::logOut() {
  initOperation("logOut");

  ReturnStatus ret = Manager->logOut();
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOut", ret);
    return;
  }

  completeOperation("logOut");
}

void AsyncProductionManager::requestBox() {
  initOperation("requestBox");

  ReturnStatus ret = Manager->requestBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("requestBox", ret);
    return;
  }

  completeOperation("requestBox");
}

void AsyncProductionManager::refundCurrentBox() {
  initOperation("refundCurrentBox");

  ReturnStatus ret = Manager->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("refundCurrentBox", ret);
    return;
  }

  completeOperation("refundCurrentBox");
}

void AsyncProductionManager::completeCurrentBox() {
  initOperation("completeCurrentBox");

  ReturnStatus ret = Manager->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("completeCurrentBox", ret);
    return;
  }

  completeOperation("completeCurrentBox");
}

void AsyncProductionManager::releaseTransponder() {
  initOperation("releaseTransponder");

  ReturnStatus ret = Manager->releaseTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }

  completeOperation("releaseTransponder");
}

void AsyncProductionManager::rereleaseTransponder(
    const StringDictionary& param) {
  initOperation("rereleaseTransponder");

  ReturnStatus ret = Manager->rereleaseTransponder(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  completeOperation("rereleaseTransponder");
}

void AsyncProductionManager::rollbackTransponder() {
  initOperation("rollbackTransponder");

  ReturnStatus ret = Manager->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("rollbackTransponder", ret);
    return;
  }

  completeOperation("rollbackTransponder");
}

void AsyncProductionManager::createManager() {
  NamedObjectFactory factory(thread());

  std::shared_ptr<IStickerPrinter> sp =
      factory.createShared<TE310Printer>("TE310Printer");

  std::shared_ptr<IProgrammer> p =
      factory.createShared<JLinkExeProgrammer>("JLinkExeProgrammer");

  std::shared_ptr<IServerConnection> sc =
      factory.createShared<PersoServerConnection>("PersoServerConnection");

  Manager = std::make_unique<ProductionManager>("ProductionManager", sc, sp, p);
}
