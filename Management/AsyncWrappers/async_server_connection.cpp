#include "async_server_connection.h"
#include "global_environment.h"
#include "production_manager_gui_subkernel.h"

#include <QDir>
#include <QSettings>

AsyncServerConnection::AsyncServerConnection(
    const QString& name,
    std::shared_ptr<IServerConnection> sc)
    : ProgressableAsyncWrapper{name}, Server(sc) {}

void AsyncServerConnection::connect() {
  initOperation("connectToServer");

  ReturnStatus ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    processOperationError("connectToServer", ret);
    return;
  }

  completeOperation("connectToServer");
}

void AsyncServerConnection::disconnect() {
  initOperation("connectToServer");

  Server->disconnect();

  completeOperation("connectToServer");
}

void AsyncServerConnection::launchProductionLine(
    const StringDictionary& param) {
  initOperation("launchProductionLine");

  ReturnStatus ret = Server->launchProductionLine(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("launchProductionLine", ret);
    return;
  }

  completeOperation("launchProductionLine");
}

void AsyncServerConnection::shutdownProductionLine() {
  initOperation("shutdownProductionLine");

  Server->shutdownProductionLine();

  completeOperation("shutdownProductionLine");
}

void AsyncServerConnection::getProductionLineData() {
  initOperation("getProductionLineData");

  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary pldata;

  ret = Server->getProductionLineData(pldata);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getProductionLineData", ret);
    return;
  }

  emit productionLineDataReady(pldata);

  completeOperation("getProductionLineData");
}

void AsyncServerConnection::echo() {
  initOperation("echoServer");

  ReturnStatus ret;
  ret = Server->echo();
  if (ret != ReturnStatus::NoError) {
    processOperationError("echoServer", ret);
    return;
  }

  completeOperation("echoServer");
}

void AsyncServerConnection::requestBox() {
  initOperation("requestBox");

  ReturnStatus ret = Server->requestBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("requestBox", ret);
    return;
  }

  completeOperation("requestBox");
}

void AsyncServerConnection::getCurrentBoxData() {
  initOperation("getCurrentBoxData");

  StringDictionary bdata;
  ReturnStatus ret = Server->getCurrentBoxData(bdata);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getCurrentBoxData", ret);
    return;
  }

  emit boxDataReady(bdata);

  completeOperation("getCurrentBoxData");
}

void AsyncServerConnection::refundCurrentBox() {
  initOperation("refundCurrentBox");

  ReturnStatus ret = Server->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("refundCurrentBox", ret);
    return;
  }

  completeOperation("refundCurrentBox");
}

void AsyncServerConnection::completeCurrentBox() {
  initOperation("completeCurrentBox");

  ReturnStatus ret = Server->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("completeCurrentBox", ret);
    return;
  }

  completeOperation("completeCurrentBox");
}

void AsyncServerConnection::releaseTransponder() {
  initOperation("releaseTransponder");

  StringDictionary result;
  ReturnStatus ret = Server->releaseTransponder(result);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }

  completeOperation("releaseTransponder");
}

void AsyncServerConnection::confirmTransponderRelease(
    const StringDictionary& param) {
  initOperation("confirmTransponderRelease");

  ReturnStatus ret = Server->confirmTransponderRelease(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("confirmTransponderRelease", ret);
    return;
  }

  completeOperation("confirmTransponderRelease");
}

void AsyncServerConnection::rereleaseTransponder(
    const StringDictionary& param) {
  initOperation("rereleaseTransponder");

  StringDictionary result;
  ReturnStatus ret = Server->rereleaseTransponder(param, result);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  completeOperation("rereleaseTransponder");
}

void AsyncServerConnection::rollbackTransponder() {
  initOperation("rollbackTransponder");

  ReturnStatus ret = Server->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("rollbackTransponder", ret);
    return;
  }

  completeOperation("rollbackTransponder");
}

void AsyncServerConnection::getCurrentTransponderData() {
  initOperation("getCurrentTransponderData");

  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary tdata;

  ret = Server->getCurrentTransponderData(tdata);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getCurrentTransponderData", ret);
    return;
  }

  emit transponderDataReady(tdata);

  completeOperation("getCurrentTransponderData");
}

void AsyncServerConnection::getTransponderData(const StringDictionary& param) {
  initOperation("getTransponderData");

  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary tdata;

  ret = Server->getTransponderData(param, tdata);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getTransponderData", ret);
    return;
  }

  emit transponderDataReady(tdata);

  completeOperation("getTransponderData");
}

void AsyncServerConnection::printBoxSticker(const StringDictionary& param) {
  initOperation("printBoxSticker");

  ReturnStatus ret;
  ret = Server->printBoxSticker(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printBoxSticker", ret);
    return;
  }

  completeOperation("printBoxSticker");
}

void AsyncServerConnection::printLastBoxSticker() {
  initOperation("printLastBoxSticker");

  ReturnStatus ret;
  ret = Server->printLastBoxSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastBoxSticker", ret);
    return;
  }

  completeOperation("printLastBoxSticker");
}

void AsyncServerConnection::printPalletSticker(const StringDictionary& param) {
  initOperation("printPalletSticker");

  ReturnStatus ret;
  ret = Server->printPalletSticker(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printPalletSticker", ret);
    return;
  }

  completeOperation("printPalletSticker");
}

void AsyncServerConnection::printLastPalletSticker() {
  initOperation("printLastPalletSticker");

  ReturnStatus ret;
  ret = Server->printLastPalletSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastPalletSticker", ret);
    return;
  }

  completeOperation("printLastPalletSticker");
}

void AsyncServerConnection::connectDependecies() {
  ProductionManagerGuiSubkernel* augs =
      GlobalEnvironment::instance()->getObject<ProductionManagerGuiSubkernel>(
          "ProductionManagerGuiSubkernel");

  QObject::connect(this, &AsyncServerConnection::productionLineDataReady, augs,
                   &ProductionManagerGuiSubkernel::displayProductionLineData);
  QObject::connect(this, &AsyncServerConnection::boxDataReady, augs,
                   &ProductionManagerGuiSubkernel::displayBoxData);
  QObject::connect(this, &AsyncServerConnection::transponderDataReady, augs,
                   &ProductionManagerGuiSubkernel::displayTransponderData);
  QObject::connect(this, &AsyncServerConnection::firwareReady, augs,
                   &ProductionManagerGuiSubkernel::displayFirmware);
}
