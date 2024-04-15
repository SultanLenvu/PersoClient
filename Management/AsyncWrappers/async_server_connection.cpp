#include "async_server_connection.h"
#include "global_environment.h"
#include "named_object_factory.h"
#include "perso_server_connection.h"
#include "production_manager_gui_subkernel.h"

#include <QDir>
#include <QSettings>

AsyncServerConnection::AsyncServerConnection(const QString& name)
    : ProgressableAsyncWrapper{name} {
  NamedObjectFactory factory(thread());
  Context =
      factory.createShared<ProductionUnitContext>("ProductionUnitContext");

  Server = factory.createShared<PersoServerConnection>("PersoServerConnection");
}

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
  StringDictionary data;

  ret = Server->getProductionLineData(data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getProductionLineData", ret);
    return;
  }
  Context->setState(std::move(data));

  completeOperation("getProductionLineData");
}

void AsyncServerConnection::echo() {
  initOperation("echoServer");

  ReturnStatus ret = ReturnStatus::NoError;
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

  StringDictionary data;
  ReturnStatus ret = Server->getCurrentBoxData(data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getCurrentBoxData", ret);
    return;
  }
  Context->setState(std::move(data));

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

  StringDictionary data;

  ReturnStatus ret = Server->releaseTransponder(data);
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

  StringDictionary data;
  ReturnStatus ret = Server->rereleaseTransponder(param, data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }
  Context->setTransponder(std::move(data));

  completeOperation("rereleaseTransponder");
}

void AsyncServerConnection::confirmTransponderRerelease(
    const StringDictionary& param) {
  initOperation("confirmTransponderRerelease");

  ReturnStatus ret = Server->confirmTransponderRerelease(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("confirmTransponderRerelease", ret);
    return;
  }

  completeOperation("confirmTransponderRerelease");
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
  StringDictionary data;

  ret = Server->getCurrentTransponderData(data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getCurrentTransponderData", ret);
    return;
  }
  Context->setTransponder(std::move(data));

  completeOperation("getCurrentTransponderData");
}

void AsyncServerConnection::getTransponderData(const StringDictionary& param) {
  initOperation("getTransponderData");

  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary data;

  ret = Server->getTransponderData(param, data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("getTransponderData", ret);
    return;
  }
  Context->setTransponder(std::move(data));

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
}
