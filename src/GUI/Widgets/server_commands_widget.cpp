#include "server_commands_widget.h"
#include "authorization_dialog.h"
#include "string_input_dialog.h"
#include "transponder_sticker_scan_dialog.h"
#include "ucid_checker.h"

ServerCommandsWidget::ServerCommandsWidget(QWidget* parent)
    : QGroupBox{parent} {
  create();
  connectInternals();
  createCommandMethod();
}

void ServerCommandsWidget::create() {
  setTitle("Клиентские запросы");

  Layout = new QVBoxLayout();
  setLayout(Layout);

  ComboBox = new QComboBox();
  Layout->addWidget(ComboBox);
  ComboBox->addItem("connect");
  ComboBox->addItem("disconnect");

  ComboBox->addItem("echo");
  ComboBox->addItem("launch_production_line");
  ComboBox->addItem("shutdown_production_line");
  ComboBox->addItem("get_production_line_data");

  ComboBox->addItem("request_box");
  ComboBox->addItem("get_current_box_data");
  ComboBox->addItem("refund_current_box");
  ComboBox->addItem("complete_current_box");

  ComboBox->addItem("release_transponder");
  ComboBox->addItem("confirm_transponder_release");
  ComboBox->addItem("rerelease_transponder");
  ComboBox->addItem("confirm_transponder_rerelease");
  ComboBox->addItem("rollback_tranponder");
  ComboBox->addItem("get_current_transponder_data");
  ComboBox->addItem("get_transponder_data");

  ComboBox->addItem("print_box_sticker");
  ComboBox->addItem("print_last_box_sticker");
  ComboBox->addItem("print_pallet_sticker");
  ComboBox->addItem("print_last_pallet_sticker");

  Button = new QPushButton("Выполнить");
  Layout->addWidget(Button);
  QObject::connect(Button, &QPushButton::clicked, this,
                   &ServerCommandsWidget::prepareCommandExecution);
}

void ServerCommandsWidget::connectInternals() {}

void ServerCommandsWidget::createCommandMethod() {
  CommandMethods["connect"] = &ServerCommandsWidget::connect;
  CommandMethods["disconnect"] = &ServerCommandsWidget::disconnect;

  CommandMethods["echo"] = &ServerCommandsWidget::echo;
  CommandMethods["launch_production_line"] =
      &ServerCommandsWidget::launchProductionLine;
  CommandMethods["shutdown_production_line"] =
      &ServerCommandsWidget::shutdownProductionLine;
  CommandMethods["get_production_line_data"] =
      &ServerCommandsWidget::getProductionLineData;

  CommandMethods["request_box"] = &ServerCommandsWidget::requestBox;
  CommandMethods["get_current_box_data"] =
      &ServerCommandsWidget::getCurrentBoxData;
  CommandMethods["refund_current_box"] =
      &ServerCommandsWidget::refundCurrentBox;
  CommandMethods["complete_current_box"] =
      &ServerCommandsWidget::completeCurrentBox;

  CommandMethods["release_transponder"] =
      &ServerCommandsWidget::getTransponderFirmware;
  CommandMethods["confirm_transponder_release"] =
      &ServerCommandsWidget::confirmTransponderFirmware;
  CommandMethods["rerelease_transponder"] =
      &ServerCommandsWidget::repeatTransponderFirmware;
  CommandMethods["confirm_transponder_rerelease"] =
      &ServerCommandsWidget::confirmRepeatedTransponderFirmware;
  CommandMethods["rollback_transponder"] =
      &ServerCommandsWidget::rollbackTransponder;
  CommandMethods["get_current_transponder_data"] =
      &ServerCommandsWidget::getCurrentTransponderData;
  CommandMethods["get_transponder_data"] =
      &ServerCommandsWidget::getTransponderData;

  CommandMethods["print_box_sticker"] = &ServerCommandsWidget::printBoxSticker;
  CommandMethods["print_last_box_sticker"] =
      &ServerCommandsWidget::printLastBoxSticker;
  CommandMethods["print_pallet_sticker"] =
      &ServerCommandsWidget::printPalletSticker;
  CommandMethods["print_last_pallet_sticker"] =
      &ServerCommandsWidget::printLastPalletSticker;
}

void ServerCommandsWidget::prepareCommandExecution() {
  (this->*CommandMethods.value(ComboBox->currentText()))();
}

void ServerCommandsWidget::connect() {
  emit connect_trigger();
}

void ServerCommandsWidget::disconnect() {
  emit disconnect_trigger();
}

void ServerCommandsWidget::echo() {
  emit echo_trigger();
}

void ServerCommandsWidget::launchProductionLine() {
  StringDictionary param;

  AuthorizationDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit launchProductionLine_trigger(param);
}

void ServerCommandsWidget::shutdownProductionLine() {
  emit shutdownProductionLine_trigger();
}

void ServerCommandsWidget::getProductionLineData() {
  emit getProductionLineData_trigger();
}

void ServerCommandsWidget::requestBox() {
  emit requestBox_trigger();
}

void ServerCommandsWidget::getCurrentBoxData() {
  emit getCurrentBoxData_trigger();
}

void ServerCommandsWidget::refundCurrentBox() {
  emit refundCurrentBox_trigger();
}

void ServerCommandsWidget::completeCurrentBox() {
  emit connect_trigger();
}

void ServerCommandsWidget::getTransponderFirmware() {
  emit getTransponderFirmware_trigger();
}

void ServerCommandsWidget::confirmTransponderFirmware() {
  StringDictionary param;

  UcidChecker checker;
  StringInputDialog dialog("transponder_ucid", &checker);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit confirmTransponderFirmware_trigger(param);
}

void ServerCommandsWidget::repeatTransponderFirmware() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit repeatTransponderFirmware_trigger(param);
}

void ServerCommandsWidget::confirmRepeatedTransponderFirmware() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit confirmRepeatedTransponderFirmware_trigger(param);
}

void ServerCommandsWidget::rollbackTransponder() {
  emit rollbackTransponder_trigger();
}

void ServerCommandsWidget::getCurrentTransponderData() {
  emit getCurrentTransponderData_trigger();
}

void ServerCommandsWidget::getTransponderData() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit getTransponderData_trigger(param);
}

void ServerCommandsWidget::printBoxSticker() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit printBoxSticker_trigger(param);
}

void ServerCommandsWidget::printLastBoxSticker() {
  emit printLastBoxSticker_trigger();
}

void ServerCommandsWidget::printPalletSticker() {
  StringDictionary param;

  StringInputDialog dialog("transponder_pan");
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit printPalletSticker_trigger(param);
}

void ServerCommandsWidget::printLastPalletSticker() {
  emit printLastPalletSticker_trigger();
}
