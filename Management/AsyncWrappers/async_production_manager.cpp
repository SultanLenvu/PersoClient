#include "async_production_manager.h"

ProductionManagerAsyncWrapper::ProductionManagerAsyncWrapper(
    const QString& name)
    : ProgressableAsyncWrapper(name) {}

void ProductionManagerAsyncWrapper::logOn(const StringDictionary& param) {
  initOperation("logOn");

  ReturnStatus ret;
  ret = Manager->logOn(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOn", ret);
    return;
  }

  completeOperation("logOn");
}

void ProductionManagerAsyncWrapper::logOut() {
  initOperation("logOut");

  ReturnStatus ret = Manager->logOut();
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOut", ret);
    return;
  }

  completeOperation("logOut");
}

void ProductionManagerAsyncWrapper::requestBox() {
  initOperation("requestBox");

  ReturnStatus ret = Manager->requestBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("requestBox", ret);
    return;
  }

  completeOperation("requestBox");
}

void ProductionManagerAsyncWrapper::refundCurrentBox() {
  initOperation("refundCurrentBox");

  ReturnStatus ret = Manager->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("refundCurrentBox", ret);
    return;
  }

  completeOperation("refundCurrentBox");
}

void ProductionManagerAsyncWrapper::completeCurrentBox() {
  initOperation("completeCurrentBox");

  ReturnStatus ret = Manager->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("completeCurrentBox", ret);
    return;
  }

  completeOperation("completeCurrentBox");
}

void ProductionManagerAsyncWrapper::releaseTransponder() {
  initOperation("releaseTransponder");

  ReturnStatus ret = Manager->releaseTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }

  completeOperation("releaseTransponder");
}

void ProductionManagerAsyncWrapper::rereleaseTransponder(
    const StringDictionary& param) {
  initOperation("rereleaseTransponder");

  ReturnStatus ret = Manager->rereleaseTransponder(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  completeOperation("rereleaseTransponder");
}

void ProductionManagerAsyncWrapper::rollbackTransponder() {
  initOperation("rollbackTransponder");

  ReturnStatus ret = Manager->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("rollbackTransponder", ret);
    return;
  }

  completeOperation("rollbackTransponder");
}
