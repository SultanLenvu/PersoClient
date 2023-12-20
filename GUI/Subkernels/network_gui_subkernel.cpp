#include "network_gui_subkernel.h"
#include "authorization_gui.h"
#include "master_gui.h"
#include "production_gui.h"
#include "testing_gui.h"
#include "transponder_sticker_scan_dialog.h"

NetworkGuiSubkernel::NetworkGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
  TransponderDataModel =
      std::unique_ptr<HashModel>(new HashModel("TransponderData"));
}

NetworkGuiSubkernel::~NetworkGuiSubkernel() {}

void NetworkGuiSubkernel::setCurrentGui(std::shared_ptr<AbstractGui> gui) {
  CurrentGui = gui;

  switch (gui->type()) {
    case AbstractGui::Production:
      connectProductionGui();
      break;
    case AbstractGui::Testing:
      connectTestingGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    default:
      break;
  }
}

void NetworkGuiSubkernel::setManager(std::shared_ptr<AbstractManager> manager) {
  Manager = manager;

  connect(this, &NetworkGuiSubkernel::performServerConnecting_signal, Manager,
          &ClientManager::performServerConnecting);
  connect(this, &NetworkGuiSubkernel::performServerDisconnecting_signal,
          Manager, &ClientManager::performServerDisconnecting);
  connect(this, &NetworkGuiSubkernel::performServerEcho_signal, Manager,
          &ClientManager::performServerEcho);
  connect(this, &NetworkGuiSubkernel::performServerAuthorization_signal,
          Manager, &ClientManager::performServerAuthorization);
  connect(this, &NetworkGuiSubkernel::performTransponderFirmwareLoading_signal,
          Manager, &ClientManager::performTransponderFirmwareLoading);
  connect(this,
          &NetworkGuiSubkernel::performTransponderFirmwareReloading_signal,
          Manager, &ClientManager::performTransponderFirmwareReloading);
  connect(this, &NetworkGuiSubkernel::rollbackProductionLine_signal, Manager,
          &ClientManager::rollbackProductionLine);
  connect(this, &NetworkGuiSubkernel::performBoxStickerPrinting_signal, Manager,
          &ClientManager::performBoxStickerPrinting);
  connect(this, &NetworkGuiSubkernel::performPalletStickerPrinting_signal,
          Manager, &ClientManager::performPalletStickerPrinting);
}

void NetworkGuiSubkernel::displayTransponderData(
    const std::shared_ptr<StringDictionary> data) {}

void NetworkGuiSubkernel::connectAuthorizationGui() {
  AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui.get());

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::authorize_guiSlot);
}

void NetworkGuiSubkernel::connectProductionGui() {
  ProductionGUI* gui = dynamic_cast<ProductionGUI*>(CurrentGui.get());

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->RollbackProductionLinePushButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::rollbackProductionLine_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printBoxSticker_guiSlot);
}

void NetworkGuiSubkernel::connectTestingGui() {
  TestingGui* gui = dynamic_cast<TestingGui*>(CurrentGui.get());

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::rereleaseTransponder_guiSlot);

  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printPalletSticker_guiSlot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel.get());
}

void NetworkGuiSubkernel::connectMasterGui() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui.get());

  connect(gui->ServerConnectPushButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::connect_guiSlot);
  connect(gui->ServerDisconnectButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::disconnect_guiSlot);
  connect(gui->ServerEchoRequestButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::echoRequest_guiSlot);
  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::authorize_guiSlot);

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->RollbackProductionLinePushButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::rollbackProductionLine_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printBoxSticker_guiSlot);
  connect(gui->PrintLastBoxStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printLastBoxSticker_guiSlot);
  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printPalletSticker_guiSlot);
  connect(gui->PrintLastPalletStickerButton, &QPushButton::clicked, this,
          &NetworkGuiSubkernel::printLastPalletSticker_guiSlot);
}

void NetworkGuiSubkernel::connect_guiSlot() {
  emit loggerClear_signal();

  emit connectToServer_signal();
}

void NetworkGuiSubkernel::disconnect_guiSlot() {
  emit loggerClear_signal();

  emit disconnectFromServer_signal();
}

void NetworkGuiSubkernel::echoRequest_guiSlot() {
  emit loggerClear_signal();

  emit echoServer_signal();
}

void NetworkGuiSubkernel::authorize_guiSlot() {
  AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui.get());

  if (gui->ModeChoice->currentText() == "Производство") {
    std::shared_ptr<StringDictionary> param(new StringDictionary());
    param->insert("login", gui->LoginLineEdit->text());
    param->insert("password", gui->PasswordLineEdit->text());

    emit authorize_signal(param);
    return;
  }

  if (gui->ModeChoice->currentText() == "Тестирование") {
    emit displayTestingGui();
    return;
  }
}

void NetworkGuiSubkernel::releaseTransponder_guiSlot() {
  emit loggerClear_signal();

  emit releaseTransponder_signal();
}

void NetworkGuiSubkernel::rereleaseTransponder_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit rereleaseTransponder_signal(param);
}

void NetworkGuiSubkernel::rollbackProductionLine_guiSlot() {
  emit loggerClear_signal();

  emit rollbackProductionLine_signal();
}

void NetworkGuiSubkernel::printBoxSticker_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printBoxSticker_signal(param);
}

void NetworkGuiSubkernel::printLastBoxSticker_guiSlot() {
  emit loggerClear_signal();

  emit printLastBoxSticker_signal();
}

void NetworkGuiSubkernel::printPalletSticker_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printPalletSticker_signal(param);
}

void NetworkGuiSubkernel::printLastPalletSticker_guiSlot() {
  emit loggerClear_signal();

  emit printLastPalletSticker_signal();
}
