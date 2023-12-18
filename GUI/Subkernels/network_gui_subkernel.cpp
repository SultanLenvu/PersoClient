#include "network_gui_subkernel.h"

#include "master_gui.h"
#include "production_gui.h"
#include "testing_gui.h"

NetworkGuiSubkernel::NetworkGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

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
}

void NetworkGuiSubkernel::connectProductionGui() {
  ProductionGUI* gui = dynamic_cast<ProductionGUI*>(CurrentGui.get());
}

void NetworkGuiSubkernel::connectTestingGui() {}

void NetworkGuiSubkernel::connectMasterGui() {}

void NetworkGuiSubkernel::connectPushButton_slot() {
  emit loggerClear_signal();

  emit connectToServer_signal();
}

void NetworkGuiSubkernel::disconnectButton_slot() {
  emit loggerClear_signal();

  emit disconnectFromServer_signal();
}

void NetworkGuiSubkernel::echoRequestButton_slot() {
  emit loggerClear_signal();

  emit echoServer_signal();
}

void NetworkGuiSubkernel::authorizePushButton_slot() {
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);

  if (gui->ModeChoice->currentText() == "Производство") {
    QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);
    AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);
    data->insert("login", gui->LoginLineEdit->text());
    data->insert("password", gui->PasswordLineEdit->text());

    emit performServerAuthorization_signal(data);
  }

  if (gui->ModeChoice->currentText() == "Тестирование") {
    createTestingInterface();
  }
}

void NetworkGuiSubkernel::loadTransponderFirmwareButton_slot() {}

void NetworkGuiSubkernel::reloadTransponderFirmwareButton_slot() {}

void NetworkGuiSubkernel::rollbackProductionLinePushButton_slot() {}

void NetworkGuiSubkernel::printBoxStickerButton_slot() {}

void NetworkGuiSubkernel::printPalletStickerButton_slot() {}
