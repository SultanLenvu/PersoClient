#include "production_gui_subkernel.h"
#include "authorization_gui.h"
#include "master_gui.h"
#include "production_assembler_gui.h"
#include "production_tester_gui.h"
#include "transponder_sticker_scan_dialog.h"

ProductionGuiSubkernel::ProductionGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
  TransponderDataModel =
      std::unique_ptr<HashModel>(new HashModel("TransponderData"));

  CurrentRole = Assembler;
}

ProductionGuiSubkernel::~ProductionGuiSubkernel() {}

void ProductionGuiSubkernel::connectAuthorizationGui(
    std::shared_ptr<AuthorizationGui> gui) {
  CurrentGui = gui;

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::authorize_guiSlot);
}

void ProductionGuiSubkernel::connectMasterGui(std::shared_ptr<MasterGui> gui) {
  CurrentGui = gui;

  connect(gui->ServerConnectPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::connect_guiSlot);
  connect(gui->ServerDisconnectButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::disconnect_guiSlot);
  connect(gui->ServerEchoRequestButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::echoRequest_guiSlot);
  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::authorize_guiSlot);

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->rollbackTransponderPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rollbackTransponder_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printBoxSticker_guiSlot);
  connect(gui->PrintLastBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printLastBoxSticker_guiSlot);
  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printPalletSticker_guiSlot);
  connect(gui->PrintLastPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printLastPalletSticker_guiSlot);
}

void ProductionGuiSubkernel::connectProductionAssemblerGui(
    std::shared_ptr<ProductionAssemblerGui> gui) {
  CurrentGui = gui;

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->rollbackTransponderPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rollbackTransponder_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printBoxSticker_guiSlot);
}

void ProductionGuiSubkernel::connectProductionTesterGui(
    std::shared_ptr<ProductionTesterGui> gui) {
  CurrentGui = gui;

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);

  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printPalletSticker_guiSlot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel.get());
}

void ProductionGuiSubkernel::resetCurrentGui() {
  CurrentGui.reset();
}

void ProductionGuiSubkernel::connectManager(
    const ProductionManager* manager) const {
  connect(this, &ProductionGuiSubkernel::authorize_signal, manager,
          &ProductionManager::initServerConnection);
  connect(manager, &ProductionManager::authorizationCompleted, this,
          &ProductionGuiSubkernel::authorizationCompleted_slot);
  connect(this, &ProductionGuiSubkernel::disconnectFromServer_signal, manager,
          &ProductionManager::resetServerConnection);

  connect(this, &ProductionGuiSubkernel::echoServer_signal, manager,
          &ProductionManager::echoServer);

  connect(this, &ProductionGuiSubkernel::releaseTransponder_signal, manager,
          &ProductionManager::releaseTransponder);
  connect(this, &ProductionGuiSubkernel::rereleaseTransponder_signal, manager,
          &ProductionManager::rereleaseTransponder);
  connect(this, &ProductionGuiSubkernel::rollbackTransponder_signal, manager,
          &ProductionManager::rollbackTransponder);

  connect(this, &ProductionGuiSubkernel::printBoxSticker_signal, manager,
          &ProductionManager::printBoxSticker);
  connect(this, &ProductionGuiSubkernel::printLastBoxSticker_signal, manager,
          &ProductionManager::printLastBoxSticker);
  connect(this, &ProductionGuiSubkernel::printPalletSticker_signal, manager,
          &ProductionManager::printPalletSticker);
  connect(this, &ProductionGuiSubkernel::printLastPalletSticker_signal, manager,
          &ProductionManager::printLastPalletSticker);
}

void ProductionGuiSubkernel::authorizationCompleted_slot() {
  if (CurrentRole == Assembler) {
    emit displayProductionAssemblerGui();
  }

  if (CurrentRole == Tester) {
    emit displayProductionTesterGui();
  }
}

void ProductionGuiSubkernel::displayTransponderData(
    const std::shared_ptr<StringDictionary> data) {
  TransponderDataModel->buildTransponderData(data.get());
}

void ProductionGuiSubkernel::connect_guiSlot() {
  emit loggerClear_signal();

  emit connectToServer_signal();
}

void ProductionGuiSubkernel::disconnect_guiSlot() {
  emit loggerClear_signal();

  emit disconnectFromServer_signal();
}

void ProductionGuiSubkernel::echoRequest_guiSlot() {
  emit loggerClear_signal();

  emit echoServer_signal();
}

void ProductionGuiSubkernel::authorize_guiSlot() {
  AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui.get());

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  param->insert("login", gui->LoginLineEdit->text());
  param->insert("password", gui->PasswordLineEdit->text());

  emit authorize_signal(param);

  if (gui->ModeChoice->currentText() == "Сборка") {
    CurrentRole = Assembler;
    return;
  }

  if (gui->ModeChoice->currentText() == "Тестирование") {
    CurrentRole = Tester;
    return;
  }
}

void ProductionGuiSubkernel::releaseTransponder_guiSlot() {
  emit loggerClear_signal();

  emit releaseTransponder_signal();
}

void ProductionGuiSubkernel::rereleaseTransponder_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit rereleaseTransponder_signal(param);
}

void ProductionGuiSubkernel::rollbackTransponder_guiSlot() {
  emit loggerClear_signal();

  emit rollbackTransponder_signal();
}

void ProductionGuiSubkernel::printBoxSticker_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printBoxSticker_signal(param);
}

void ProductionGuiSubkernel::printLastBoxSticker_guiSlot() {
  emit loggerClear_signal();

  emit printLastBoxSticker_signal();
}

void ProductionGuiSubkernel::printPalletSticker_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printPalletSticker_signal(param);
}

void ProductionGuiSubkernel::printLastPalletSticker_guiSlot() {
  emit loggerClear_signal();

  emit printLastPalletSticker_signal();
}
