#include "server_connection_gui_subkernel.h"
#include "authorization_dialog.h"
#include "string_input_dialog.h"
#include "transponder_sticker_scan_dialog.h"
#include "ucid_checker.h"

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

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit getTransponderData_signal(param);
}

void ServerConnectionGuiSubkernel::printBoxSticker() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
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

void ServerConnectionGuiSubkernel::generateDisconnectionAlert() {
  QMessageBox::critical(nullptr, "Ошибка", "Соединение с сервером оборвалось.",
                        QMessageBox::Ok);
}
