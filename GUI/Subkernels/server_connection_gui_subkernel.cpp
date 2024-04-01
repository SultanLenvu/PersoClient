#include "server_connection_gui_subkernel.h"
#include "async_server_connection.h"
#include "authorization_dialog.h"
#include "global_environment.h"
#include "numeric_string_checker.h"
#include "transponder_sticker_scan_dialog.h"

ServerConnectionGuiSubkernel::ServerConnectionGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel(name) {
  connectDependecies();
  createCommandMethod();
}

void ServerConnectionGuiSubkernel::executeCommand(const QString& name) {
  emit clearLogDisplay_signal();

  (this->*CommandMethods.value(name))();
}

void ServerConnectionGuiSubkernel::onServerDisconnected() {
  QMessageBox::critical(nullptr, "Ошибка", "Соединение с сервером оборвалось.",
                        QMessageBox::Ok);
}

void ServerConnectionGuiSubkernel::connect() {
  emit clearLogDisplay_signal();
  emit connect_signal();
}

void ServerConnectionGuiSubkernel::disconnect() {
  emit clearLogDisplay_signal();
  emit disconnect_signal();
}

void ServerConnectionGuiSubkernel::echo() {
  emit clearLogDisplay_signal();
  emit echo_signal();
}

void ServerConnectionGuiSubkernel::launchProductionLine() {
  StringDictionary param;

  AuthorizationDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit launchProductionLine_signal(param);
}

void ServerConnectionGuiSubkernel::shutdownProductionLine() {
  emit clearLogDisplay_signal();
  emit shutdownProductionLine_signal();
}

void ServerConnectionGuiSubkernel::getProductionLineData() {
  emit clearLogDisplay_signal();
  emit getProductionLineData_signal();
}

void ServerConnectionGuiSubkernel::requestBox() {
  emit clearLogDisplay_signal();
  emit requestBox_signal();
}

void ServerConnectionGuiSubkernel::getCurrentBoxData() {
  emit clearLogDisplay_signal();
  emit getCurrentBoxData_signal();
}

void ServerConnectionGuiSubkernel::refundCurrentBox() {
  emit clearLogDisplay_signal();
  emit refundCurrentBox_signal();
}

void ServerConnectionGuiSubkernel::completeCurrentBox() {
  emit clearLogDisplay_signal();
  emit completeCurrentBox_signal();
}

void ServerConnectionGuiSubkernel::releaseTransponder() {
  emit clearLogDisplay_signal();
  emit releaseTransponder_signal();
}

void ServerConnectionGuiSubkernel::confirmTransponderRelease() {
  StringDictionary param;

  UcidChecker checker;
  StringInputDialog dialog("transponder_ucid", &checker);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit confirmTransponderRelease_signal(param);
  emit clearLogDisplay_signal();
}

void ServerConnectionGuiSubkernel::rereleaseTransponder() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit rereleaseTransponder_signal(param);
}

void ServerConnectionGuiSubkernel::confirmTransponderRerelease() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit confirmTransponderRerelease_signal(param);
}

void ServerConnectionGuiSubkernel::rollbackTransponder() {
  emit clearLogDisplay_signal();
  emit rollbackTransponder_signal();
}

void ServerConnectionGuiSubkernel::getCurrentTransponderData() {
  emit clearLogDisplay_signal();
  emit getCurrentTransponderData_signal();
}

void ServerConnectionGuiSubkernel::getTransponderData() {
  StringDictionary param;

  StringInputDialog dialog("transponder_pan");
  NumericStringChecker checker;
  dialog.setChecker(&checker);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit getTransponderData_signal(param);
}

void ServerConnectionGuiSubkernel::printBoxSticker() {
  StringDictionary param;

  StringInputDialog dialog("transponder_pan");
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit printBoxSticker_signal(param);
}

void ServerConnectionGuiSubkernel::printLastBoxSticker() {
  emit clearLogDisplay_signal();
  emit printLastBoxSticker_signal();
}

void ServerConnectionGuiSubkernel::printPalletSticker() {
  StringDictionary param;

  StringInputDialog dialog("transponder_pan");
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit printPalletSticker_signal(param);
}

void ServerConnectionGuiSubkernel::printLastPalletSticker() {
  emit clearLogDisplay_signal();
  emit printLastPalletSticker_signal();
}

void ServerConnectionGuiSubkernel::connectDependecies() {
  AsyncServerConnection* psm =
      GlobalEnvironment::instance()->getObject<AsyncServerConnection>(
          "AsyncServerConnection");

  // К менеджерам
  QObject::connect(this, &ServerConnectionGuiSubkernel::connect_signal, psm,
                   &AsyncServerConnection::connect);
  QObject::connect(this, &ServerConnectionGuiSubkernel::disconnect_signal, psm,
                   &AsyncServerConnection::disconnect);

  QObject::connect(this, &ServerConnectionGuiSubkernel::echo_signal, psm,
                   &AsyncServerConnection::echo);

  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::launchProductionLine_signal,
                   psm, &AsyncServerConnection::launchProductionLine);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::shutdownProductionLine_signal,
                   psm, &AsyncServerConnection::shutdownProductionLine);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::getProductionLineData_signal,
                   psm, &AsyncServerConnection::getProductionLineData);

  QObject::connect(this, &ServerConnectionGuiSubkernel::requestBox_signal, psm,
                   &AsyncServerConnection::requestBox);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::getCurrentBoxData_signal, psm,
                   &AsyncServerConnection::getCurrentBoxData);
  QObject::connect(this, &ServerConnectionGuiSubkernel::refundCurrentBox_signal,
                   psm, &AsyncServerConnection::refundCurrentBox);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::completeCurrentBox_signal,
                   psm, &AsyncServerConnection::completeCurrentBox);

  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::releaseTransponder_signal,
                   psm, &AsyncServerConnection::releaseTransponder);
  QObject::connect(
      this, &ServerConnectionGuiSubkernel::confirmTransponderRelease_signal,
      psm, &AsyncServerConnection::confirmTransponderRelease);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::rereleaseTransponder_signal,
                   psm, &AsyncServerConnection::rereleaseTransponder);
  QObject::connect(
      this, &ServerConnectionGuiSubkernel::confirmTransponderRerelease_signal,
      psm, &AsyncServerConnection::rereleaseTransponder);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::rollbackTransponder_signal,
                   psm, &AsyncServerConnection::rollbackTransponder);
  QObject::connect(
      this, &ServerConnectionGuiSubkernel::getCurrentTransponderData_signal,
      psm, &AsyncServerConnection::getCurrentTransponderData);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::getTransponderData_signal,
                   psm, &AsyncServerConnection::getTransponderData);

  QObject::connect(this, &ServerConnectionGuiSubkernel::printBoxSticker_signal,
                   psm, &AsyncServerConnection::printBoxSticker);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::printLastBoxSticker_signal,
                   psm, &AsyncServerConnection::printLastBoxSticker);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::printPalletSticker_signal,
                   psm, &AsyncServerConnection::printPalletSticker);
  QObject::connect(this,
                   &ServerConnectionGuiSubkernel::printLastPalletSticker_signal,
                   psm, &AsyncServerConnection::printLastPalletSticker);
}

void ServerConnectionGuiSubkernel::createCommandMethod() {
  CommandMethods["connect"] = &ServerConnectionGuiSubkernel::connect;
  CommandMethods["disconnect"] = &ServerConnectionGuiSubkernel::disconnect;

  CommandMethods["echo"] = &ServerConnectionGuiSubkernel::echo;
  CommandMethods["launch_production_line"] =
      &ServerConnectionGuiSubkernel::launchProductionLine;
  CommandMethods["shutdown_production_line"] =
      &ServerConnectionGuiSubkernel::shutdownProductionLine;
  CommandMethods["get_production_line_data"] =
      &ServerConnectionGuiSubkernel::getProductionLineData;

  CommandMethods["request_box"] = &ServerConnectionGuiSubkernel::requestBox;
  CommandMethods["get_current_box_data"] =
      &ServerConnectionGuiSubkernel::getCurrentBoxData;
  CommandMethods["refund_current_box"] =
      &ServerConnectionGuiSubkernel::refundCurrentBox;
  CommandMethods["complete_current_box"] =
      &ServerConnectionGuiSubkernel::completeCurrentBox;

  CommandMethods["release_transponder"] =
      &ServerConnectionGuiSubkernel::releaseTransponder;
  CommandMethods["confirm_transponder_release"] =
      &ServerConnectionGuiSubkernel::confirmTransponderRelease;
  CommandMethods["rerelease_transponder"] =
      &ServerConnectionGuiSubkernel::rereleaseTransponder;
  CommandMethods["confirm_transponder_rerelease"] =
      &ServerConnectionGuiSubkernel::confirmTransponderRerelease;
  CommandMethods["rollback_transponder"] =
      &ServerConnectionGuiSubkernel::rollbackTransponder;
  CommandMethods["get_current_transponder_data"] =
      &ServerConnectionGuiSubkernel::getCurrentTransponderData;
  CommandMethods["get_transponder_data"] =
      &ServerConnectionGuiSubkernel::getTransponderData;

  CommandMethods["print_box_sticker"] =
      &ServerConnectionGuiSubkernel::printBoxSticker;
  CommandMethods["print_last_box_sticker"] =
      &ServerConnectionGuiSubkernel::printLastBoxSticker;
  CommandMethods["print_pallet_sticker"] =
      &ServerConnectionGuiSubkernel::printPalletSticker;
  CommandMethods["print_last_pallet_sticker"] =
      &ServerConnectionGuiSubkernel::printLastPalletSticker;
}
