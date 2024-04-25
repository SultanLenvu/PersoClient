#include "master_user_interface.h"

MasterUserInterface::MasterUserInterface(QWidget* parent) : QWidget(parent) {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  createTabs();
  createLogDisplay();

  MainLayout->setStretch(0, 5);
  MainLayout->setStretch(1, 2);
}

void MasterUserInterface::displayLog(const QString& log) {
  LogDisplay->displayLog(log);
}

void MasterUserInterface::clearLog() {
  LogDisplay->clearLogDisplay();
}

void MasterUserInterface::createTabs() {
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  createProductionManager();
  createStickerPrinter();
  createProgrammator();
}

void MasterUserInterface::createStickerPrinter() {
  StickerPrinterTab = new StickerPrinterWidget();

  connect(StickerPrinterTab,
          &StickerPrinterWidget::printLastTransponderSticker_trigger, this,
          &MasterUserInterface::printLastTransponderSticker_trigger);
  connect(StickerPrinterTab,
          &StickerPrinterWidget::printTransponderSticker_trigger, this,
          &MasterUserInterface::printTransponderSticker_trigger);
  connect(StickerPrinterTab, &StickerPrinterWidget::execCommandScript_trigger,
          this, &MasterUserInterface::execCommandScript_trigger);

  Tabs->addTab(StickerPrinterTab, "Стикер принтер");
}

void MasterUserInterface::createProgrammator() {
  ProgrammatorTab = new ProgrammatorWidget();

  connect(ProgrammatorTab, &ProgrammatorWidget::programMemory_trigger, this,
          &MasterUserInterface::programMemory_trigger);
  connect(ProgrammatorTab, &ProgrammatorWidget::readMemory_trigger, this,
          &MasterUserInterface::readMemory_trigger);
  connect(ProgrammatorTab, &ProgrammatorWidget::eraseMemory_trigger, this,
          &MasterUserInterface::eraseMemory_trigger);

  connect(ProgrammatorTab, &ProgrammatorWidget::readUserData_trigger, this,
          &MasterUserInterface::readUserData_trigger);
  connect(ProgrammatorTab, &ProgrammatorWidget::programUserData_trigger, this,
          &MasterUserInterface::programUserData_trigger);
  connect(ProgrammatorTab, &ProgrammatorWidget::readUcid_trigger, this,
          &MasterUserInterface::readUcid_trigger);

  connect(ProgrammatorTab, &ProgrammatorWidget::unlockMemory_trigger, this,
          &MasterUserInterface::unlockMemory_trigger);
  connect(ProgrammatorTab, &ProgrammatorWidget::lockMemory_trigger, this,
          &MasterUserInterface::lockMemory_trigger);

  Tabs->addTab(ProgrammatorTab, "Программатор");
}

void MasterUserInterface::createProductionManager() {
  ProductionManagerTab = new ProductionManagerWidget();

  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::connect_trigger, this,
                   &MasterUserInterface::connect_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::disconnect_trigger, this,
                   &MasterUserInterface::disconnect_trigger);
  QObject::connect(ProductionManagerTab, &ProductionManagerWidget::echo_trigger,
                   this, &MasterUserInterface::echo_trigger);

  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::launchProductionLine_trigger, this,
                   &MasterUserInterface::launchProductionLine_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::shutdownProductionLine_trigger,
                   this, &MasterUserInterface::shutdownProductionLine_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::getProductionLineData_trigger,
                   this, &MasterUserInterface::getProductionLineData_trigger);

  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::requestBox_trigger, this,
                   &MasterUserInterface::requestBox_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::getCurrentBoxData_trigger, this,
                   &MasterUserInterface::getCurrentBoxData_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::refundCurrentBox_trigger, this,
                   &MasterUserInterface::refundCurrentBox_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::completeCurrentBox_trigger, this,
                   &MasterUserInterface::completeCurrentBox_trigger);

  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::getTransponderFirmware_trigger,
                   this, &MasterUserInterface::getTransponderFirmware_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::confirmTransponderFirmware_trigger,
                   this,
                   &MasterUserInterface::confirmTransponderFirmware_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::repeatTransponderFirmware_trigger,
                   this,
                   &MasterUserInterface::repeatTransponderFirmware_trigger);
  QObject::connect(
      ProductionManagerTab,
      &ProductionManagerWidget::confirmRepeatedTransponderFirmware_trigger,
      this, &MasterUserInterface::confirmRepeatedTransponderFirmware_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::rollbackTransponder_trigger, this,
                   &MasterUserInterface::rollbackTransponder_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::getCurrentTransponderData_trigger,
                   this,
                   &MasterUserInterface::getCurrentTransponderData_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::getTransponderData_trigger, this,
                   &MasterUserInterface::getTransponderData_trigger);

  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::printBoxSticker_trigger, this,
                   &MasterUserInterface::printBoxSticker_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::printLastBoxSticker_trigger, this,
                   &MasterUserInterface::printLastBoxSticker_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::printPalletSticker_trigger, this,
                   &MasterUserInterface::printPalletSticker_trigger);
  QObject::connect(ProductionManagerTab,
                   &ProductionManagerWidget::printLastPalletSticker_trigger,
                   this, &MasterUserInterface::printLastPalletSticker_trigger);

  Tabs->addTab(ProductionManagerTab, "Программатор");
}

void MasterUserInterface::createLogDisplay() {
  LogDisplay = new LogDisplayWidget();
  MainLayout->addWidget(LogDisplay);
}
