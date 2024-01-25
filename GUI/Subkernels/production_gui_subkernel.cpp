#include "production_gui_subkernel.h"
#include "authorization_dialog.h"
#include "authorization_gui.h"
#include "master_gui.h"
#include "production_assembler_gui.h"
#include "production_manager.h"
#include "production_tester_gui.h"
#include "transponder_sticker_scan_dialog.h"

ProductionGuiSubkernel::ProductionGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
  Role = Assembler;

  createModels();
}

ProductionGuiSubkernel::~ProductionGuiSubkernel() {}

void ProductionGuiSubkernel::connectGui(AbstractGui* gui) {
  CurrentGui = gui;

  switch (CurrentGui->type()) {
    case AbstractGui::Authorization:
      connectAuthorizationGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    case AbstractGui::ProductionAssembler:
      connectProductionAssemblerGui();
      break;
    case AbstractGui::ProductionTester:
      connectProductionTesterGui();
      break;
    default:
      break;
  }
}

void ProductionGuiSubkernel::connectManager(
    std::shared_ptr<AbstractManager> manager) {
  if (manager->type() != AbstractManager::Production) {
    return;
  }

  Manager = manager;
  connectProductionManager();
}

void ProductionGuiSubkernel::reset() {
  Manager.reset();
}

void ProductionGuiSubkernel::authorizationCompleted_slot() {
  if (Role == Assembler) {
    emit displayProductionAssemblerGui();
  }

  if (Role == Tester) {
    emit displayProductionTesterGui();
  }
}

void ProductionGuiSubkernel::displayTransponderData(
    const StringDictionary& data) {
  TransponderDataModel->setData(data);
}

void ProductionGuiSubkernel::displayBoxData(const StringDictionary& data) {
  BoxDataModel->setData(data);
}

void ProductionGuiSubkernel::connectAuthorizationGui() {
  AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui);

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::authorize_guiSlot);
}

void ProductionGuiSubkernel::connectMasterGui() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui);

  connect(gui->ServerConnectPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::connect_guiSlot);
  connect(gui->ServerDisconnectButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::disconnect_guiSlot);
  connect(gui->ServerEchoRequestButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::echoRequest_guiSlot);
  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::authorize_guiSlot);

  connect(gui->RequestBoxButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::requestBox_guiSlot);
  connect(gui->GetCurrentBoxDataButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::getCurrentBoxData_guiSlot);
  connect(gui->RefundCurrentBoxButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::refundCurrentBox_guiSlot);
  connect(gui->CompleteCurrentBoxButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::completeCurrentBox_guiSlot);

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->RollbackTransponderPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rollbackTransponder_guiSlot);
  connect(gui->GetCurrentTransponderDataButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::getCurrentTransponderData_guiSlot);
  connect(gui->GetTransponderDataButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::getTransponderData_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printBoxSticker_guiSlot);
  connect(gui->PrintLastBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printLastBoxSticker_guiSlot);
  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printPalletSticker_guiSlot);
  connect(gui->PrintLastPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printLastPalletSticker_guiSlot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel.get());
  gui->BoxDataView->setModel(BoxDataModel.get());
}

void ProductionGuiSubkernel::connectProductionAssemblerGui() {
  ProductionAssemblerGui* gui =
      dynamic_cast<ProductionAssemblerGui*>(CurrentGui);

  connect(gui->ReleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::releaseTransponder_guiSlot);
  connect(gui->RereleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->RollbackTransponderPushButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rollbackTransponder_guiSlot);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printBoxSticker_guiSlot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel.get());
  gui->BoxDataView->setModel(BoxDataModel.get());
}

void ProductionGuiSubkernel::connectProductionTesterGui() {
  ProductionTesterGui* gui = dynamic_cast<ProductionTesterGui*>(CurrentGui);

  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &ProductionGuiSubkernel::printPalletSticker_guiSlot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel.get());
  //  gui->BoxDataView->setModel(BoxDataModel.get());
}

void ProductionGuiSubkernel::connectProductionManager() const {
  ProductionManager* manager = dynamic_cast<ProductionManager*>(Manager.get());

  // Сигналы от подядра
  connect(this, &ProductionGuiSubkernel::connectToServer_signal, manager,
          &ProductionManager::connectToServer);
  connect(this, &ProductionGuiSubkernel::disconnectFromServer_signal, manager,
          &ProductionManager::disconnectFromServer);
  connect(this, &ProductionGuiSubkernel::echoServer_signal, manager,
          &ProductionManager::echoServer);
  connect(this, &ProductionGuiSubkernel::authorize_signal, manager,
          &ProductionManager::launchProductionLine);

  connect(this, &ProductionGuiSubkernel::requestBox_signal, manager,
          &ProductionManager::requestBox);
  connect(this, &ProductionGuiSubkernel::getCurrentBoxData_signal, manager,
          &ProductionManager::getCurrentBoxData);
  connect(this, &ProductionGuiSubkernel::refundCurrentBox_signal, manager,
          &ProductionManager::refundCurrentBox);
  connect(this, &ProductionGuiSubkernel::completeCurrentBox_signal, manager,
          &ProductionManager::completeCurrentBox);

  connect(this, &ProductionGuiSubkernel::releaseTransponder_signal, manager,
          &ProductionManager::releaseTransponder);
  connect(this, &ProductionGuiSubkernel::rereleaseTransponder_signal, manager,
          &ProductionManager::rereleaseTransponder);
  connect(this, &ProductionGuiSubkernel::rollbackTransponder_signal, manager,
          &ProductionManager::rollbackTransponder);
  connect(this, &ProductionGuiSubkernel::getCurrentTransponderData_signal,
          manager, &ProductionManager::getCurrentTransponderData);
  connect(this, &ProductionGuiSubkernel::getTransponderData_signal, manager,
          &ProductionManager::getTransponderData);

  connect(this, &ProductionGuiSubkernel::printBoxSticker_signal, manager,
          &ProductionManager::printBoxSticker);
  connect(this, &ProductionGuiSubkernel::printLastBoxSticker_signal, manager,
          &ProductionManager::printLastBoxSticker);
  connect(this, &ProductionGuiSubkernel::printPalletSticker_signal, manager,
          &ProductionManager::printPalletSticker);
  connect(this, &ProductionGuiSubkernel::printLastPalletSticker_signal, manager,
          &ProductionManager::printLastPalletSticker);

  // Сигналы от менеджера
  connect(manager, &ProductionManager::authorizationCompleted, this,
          &ProductionGuiSubkernel::authorizationCompleted_slot);
  connect(manager, &ProductionManager::displayTransponderData_signal, this,
          &ProductionGuiSubkernel::displayTransponderData);
  connect(manager, &ProductionManager::displayBoxData_signal, this,
          &ProductionGuiSubkernel::displayBoxData);
}

void ProductionGuiSubkernel::connect_guiSlot() {
  emit clearLogDisplay_signal();

  emit connectToServer_signal();
}

void ProductionGuiSubkernel::disconnect_guiSlot() {
  emit clearLogDisplay_signal();

  emit disconnectFromServer_signal();
}

void ProductionGuiSubkernel::echoRequest_guiSlot() {
  emit clearLogDisplay_signal();

  emit echoServer_signal();
}

void ProductionGuiSubkernel::authorize_guiSlot() {
  std::shared_ptr<StringDictionary> param(new StringDictionary());

  if (CurrentGui->type() == AbstractGui::Authorization) {
    AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui);
    param->insert("login", gui->LoginLineEdit->text());
    param->insert("password", gui->PasswordLineEdit->text());
    param->insert("mode", gui->ModeChoice->currentText());
  } else {
    AuthorizationDialog dialog(CurrentGui);
    if (dialog.exec() == QDialog::Rejected) {
      return;
    }
    dialog.getData(*param);
  }

  if (param->value("mode") == "Сборка") {
    Role = Assembler;
  } else if (param->value("mode") == "Тестирование") {
    Role = Tester;
  }

  emit authorize_signal(param);
}

void ProductionGuiSubkernel::requestBox_guiSlot() {
  emit clearLogDisplay_signal();

  emit requestBox_signal();
}

void ProductionGuiSubkernel::getCurrentBoxData_guiSlot() {
  emit clearLogDisplay_signal();

  emit getCurrentBoxData_signal();
}

void ProductionGuiSubkernel::refundCurrentBox_guiSlot() {
  emit clearLogDisplay_signal();

  emit refundCurrentBox_signal();
}

void ProductionGuiSubkernel::completeCurrentBox_guiSlot() {
  emit clearLogDisplay_signal();

  emit completeCurrentBox_signal();
}

void ProductionGuiSubkernel::releaseTransponder_guiSlot() {
  emit clearLogDisplay_signal();

  emit releaseTransponder_signal();
}

void ProductionGuiSubkernel::rereleaseTransponder_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit rereleaseTransponder_signal(param);
}

void ProductionGuiSubkernel::rollbackTransponder_guiSlot() {
  emit clearLogDisplay_signal();

  emit rollbackTransponder_signal();
}

void ProductionGuiSubkernel::getCurrentTransponderData_guiSlot() {
  emit clearLogDisplay_signal();

  emit getCurrentTransponderData_signal();
}

void ProductionGuiSubkernel::getTransponderData_guiSlot() {
  std::shared_ptr<StringDictionary> param(new StringDictionary());

  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit getTransponderData_signal(param);
}

void ProductionGuiSubkernel::printBoxSticker_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printBoxSticker_signal(param);
}

void ProductionGuiSubkernel::printLastBoxSticker_guiSlot() {
  emit clearLogDisplay_signal();

  emit printLastBoxSticker_signal();
}

void ProductionGuiSubkernel::printPalletSticker_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  TransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printPalletSticker_signal(param);
}

void ProductionGuiSubkernel::printLastPalletSticker_guiSlot() {
  emit clearLogDisplay_signal();

  emit printLastPalletSticker_signal();
}

void ProductionGuiSubkernel::createModels() {
  std::shared_ptr<StringDictionary> tMatchTable(new StringDictionary());
  tMatchTable->insert("transponder_sn", "Серийный номер");
  tMatchTable->insert("transponder_ucid", "UCID");
  tMatchTable->insert("transponder_pan", "PAN");
  tMatchTable->insert("box_id", "Идентификатор бокса");
  tMatchTable->insert("transponder_release_counter", "Количество выпусков");

  TransponderDataModel = std::unique_ptr<HashTableModel>(
      new HashTableModel("TransponderDataNodel"));
  TransponderDataModel->setMatchTable(tMatchTable);

  std::shared_ptr<StringDictionary> bMatchTable(new StringDictionary());
  bMatchTable = std::shared_ptr<StringDictionary>(new StringDictionary());
  bMatchTable->insert("box_id", "Идентификатор бокса");
  bMatchTable->insert("box_quantity", "Емкость бокса");
  bMatchTable->insert("box_assembled_units", "Собрано транспондеров");
  bMatchTable->insert("box_assembling_start", "Начало сборки");
  bMatchTable->insert("box_assembling_end", "Конец сборки");
  bMatchTable->insert("pallet_id", "Идентификатор паллеты");
  bMatchTable->insert("production_line_id",
                      "Идентификатор производственной линии");

  BoxDataModel =
      std::unique_ptr<HashTableModel>(new HashTableModel("BoxDataNodel"));
  BoxDataModel->setMatchTable(bMatchTable);
}
