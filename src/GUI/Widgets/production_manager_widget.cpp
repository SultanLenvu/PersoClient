#include "production_manager_widget.h"
#include "authorization_dialog.h"

#include "transponder_sticker_scan_dialog.h"

ProductionManagerWidget::ProductionManagerWidget(QWidget* parent)
    : AbstractUserInterface{parent} {
  createWidgets();
  connectInternals();
}

void ProductionManagerWidget::setProductionLineModel(
    QAbstractItemModel* model) {
  ProductionLineDataView->setModel(model);
}

void ProductionManagerWidget::setBoxModel(QAbstractItemModel* model) {
  BoxDataView->setModel(model);
}

void ProductionManagerWidget::setTransponderModel(QAbstractItemModel* model) {
  TransponderDataView->setModel(model);
}

void ProductionManagerWidget::displayFirmware(
    const QByteArray& firmware) {
  FirmwareView->setPlainText(firmware);
}

void ProductionManagerWidget::createWidgets() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  SubLayout = new QVBoxLayout();
  MainLayout->addLayout(SubLayout);

  createCommandsWidget();
  createControlPanel();
  createDataDisplayPanel();

  // Настройка пропорции между объектами на основном макете
  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

void ProductionManagerWidget::createControlPanel() {
  ControlPanel = new QGroupBox("Команды сборочной единицы");
  SubLayout->addWidget(ControlPanel);

  ControlPanelLayout = new QVBoxLayout();
  ControlPanel->setLayout(ControlPanelLayout);

  createInitGroup();
  createBoxGroup();
  createTransponderGroup();
  createPrintingGroup();

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ControlPanelVS);
}

void ProductionManagerWidget::createCommandsWidget() {
  CommandsWidget = new ServerCommandsWidget();
  SubLayout->addWidget(CommandsWidget);

  QObject::connect(CommandsWidget, &ServerCommandsWidget::connect_trigger, this,
                   &ProductionManagerWidget::connect_trigger);
  QObject::connect(CommandsWidget, &ServerCommandsWidget::disconnect_trigger,
                   this, &ProductionManagerWidget::disconnect_trigger);
  QObject::connect(CommandsWidget, &ServerCommandsWidget::echo_trigger, this,
                   &ProductionManagerWidget::echo_trigger);

  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::launchProductionLine_trigger, this,
                   &ProductionManagerWidget::launchProductionLine_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::shutdownProductionLine_trigger, this,
                   &ProductionManagerWidget::shutdownProductionLine_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::getProductionLineData_trigger, this,
                   &ProductionManagerWidget::getProductionLineData_trigger);

  QObject::connect(CommandsWidget, &ServerCommandsWidget::requestBox_trigger,
                   this, &ProductionManagerWidget::requestBox_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::getCurrentBoxData_trigger, this,
                   &ProductionManagerWidget::getCurrentBoxData_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::refundCurrentBox_trigger, this,
                   &ProductionManagerWidget::refundCurrentBox_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::completeCurrentBox_trigger, this,
                   &ProductionManagerWidget::completeCurrentBox_trigger);

  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::getTransponderFirmware_trigger, this,
                   &ProductionManagerWidget::getTransponderFirmware_trigger);
  QObject::connect(
      CommandsWidget, &ServerCommandsWidget::confirmTransponderFirmware_trigger,
      this, &ProductionManagerWidget::confirmTransponderFirmware_trigger);
  QObject::connect(
      CommandsWidget, &ServerCommandsWidget::repeatTransponderFirmware_trigger,
      this, &ProductionManagerWidget::repeatTransponderFirmware_trigger);
  QObject::connect(
      CommandsWidget,
      &ServerCommandsWidget::confirmRepeatedTransponderFirmware_trigger, this,
      &ProductionManagerWidget::confirmRepeatedTransponderFirmware_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::rollbackTransponder_trigger, this,
                   &ProductionManagerWidget::rollbackTransponder_trigger);
  QObject::connect(
      CommandsWidget, &ServerCommandsWidget::getCurrentTransponderData_trigger,
      this, &ProductionManagerWidget::getCurrentTransponderData_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::getTransponderData_trigger, this,
                   &ProductionManagerWidget::getTransponderData_trigger);

  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::printBoxSticker_trigger, this,
                   &ProductionManagerWidget::printBoxSticker_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::printLastBoxSticker_trigger, this,
                   &ProductionManagerWidget::printLastBoxSticker_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::printPalletSticker_trigger, this,
                   &ProductionManagerWidget::printPalletSticker_trigger);
  QObject::connect(CommandsWidget,
                   &ServerCommandsWidget::printLastPalletSticker_trigger, this,
                   &ProductionManagerWidget::printLastPalletSticker_trigger);
}

void ProductionManagerWidget::createInitGroup() {
  InitGroup = new QGroupBox("Авторизация");
  ControlPanelLayout->addWidget(InitGroup);

  InitGroupLayout = new QVBoxLayout();
  InitGroup->setLayout(InitGroupLayout);

  LogOnPushButton = new QPushButton("Авторизироваться");
  InitGroupLayout->addWidget(LogOnPushButton);
  LogOutPushButton = new QPushButton("Выйти");
  InitGroupLayout->addWidget(LogOutPushButton);
}

void ProductionManagerWidget::createBoxGroup() {
  BoxGroup = new QGroupBox("Боксы");
  ControlPanelLayout->addWidget(BoxGroup);

  BoxGroupLayout = new QVBoxLayout();
  BoxGroup->setLayout(BoxGroupLayout);

  RequestBoxPushButton = new QPushButton("Запросить");
  BoxGroupLayout->addWidget(RequestBoxPushButton);
  RefundBoxPushButton = new QPushButton("Вернуть");
  BoxGroupLayout->addWidget(RefundBoxPushButton);
  CompleteCurrentBoxPushButton = new QPushButton("Завершить сборку");
  BoxGroupLayout->addWidget(CompleteCurrentBoxPushButton);
}

void ProductionManagerWidget::createTransponderGroup() {
  TransponderGroup = new QGroupBox("Транспондеры");
  ControlPanelLayout->addWidget(TransponderGroup);

  TransponderGroupLayout = new QVBoxLayout();
  TransponderGroup->setLayout(TransponderGroupLayout);

  ReleaseTransponderPushButton = new QPushButton("Выпустить");
  TransponderGroupLayout->addWidget(ReleaseTransponderPushButton);
  RereleaseTransponderPushButton = new QPushButton("Перевыпустить");
  TransponderGroupLayout->addWidget(RereleaseTransponderPushButton);
  RollbackTransponderPushButton = new QPushButton("Откатить");
  TransponderGroupLayout->addWidget(RollbackTransponderPushButton);
}

void ProductionManagerWidget::createPrintingGroup() {}

void ProductionManagerWidget::createDataDisplayPanel() {
  DataDisplayPanel = new QGroupBox("Данные");
  MainLayout->addWidget(DataDisplayPanel);

  DataDisplayLayout = new QGridLayout();
  DataDisplayPanel->setLayout(DataDisplayLayout);

  ProductionLineDataGroup = new QGroupBox("Производственная линия");
  DataDisplayLayout->addWidget(ProductionLineDataGroup, 0, 0, 1, 1);
  ProductionLineDataLayout = new QVBoxLayout();
  ProductionLineDataGroup->setLayout(ProductionLineDataLayout);
  ProductionLineDataView = new QTableView();
  ProductionLineDataLayout->addWidget(ProductionLineDataView);

  BoxDataGroup = new QGroupBox("Бокс");
  DataDisplayLayout->addWidget(BoxDataGroup, 0, 1, 1, 1);
  BoxDataLayout = new QVBoxLayout();
  BoxDataGroup->setLayout(BoxDataLayout);
  BoxDataView = new QTableView();
  BoxDataLayout->addWidget(BoxDataView);

  TransponderDataGroup = new QGroupBox("Транспондер");
  DataDisplayLayout->addWidget(TransponderDataGroup, 1, 0, 1, 1);
  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);
  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);

  FirmwareGroup = new QGroupBox("Прошивка");
  DataDisplayLayout->addWidget(FirmwareGroup, 1, 1, 1, 1);
  FirmwareLayout = new QVBoxLayout();
  FirmwareGroup->setLayout(FirmwareLayout);
  FirmwareView = new QPlainTextEdit();
  FirmwareLayout->addWidget(FirmwareView);
}

void ProductionManagerWidget::connectInternals() {
  QObject::connect(LogOnPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::logOn_clicked);
  QObject::connect(LogOutPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::logOut_trigger);

  QObject::connect(RequestBoxPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::requestBox_trigger);
  QObject::connect(RefundBoxPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::refundBox_trigger);
  QObject::connect(CompleteCurrentBoxPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::completeBox_trigger);

  QObject::connect(ReleaseTransponderPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::releaseTransponder_trigger);
  QObject::connect(RereleaseTransponderPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::rereleaseTransponder_clicked);
  QObject::connect(RollbackTransponderPushButton, &QPushButton::clicked, this,
                   &ProductionManagerWidget::rollbackTransponder_trigger);
}

void ProductionManagerWidget::logOn_clicked() {
  StringDictionary param;

  AuthorizationDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  void logOn_trigger(const StringDictionary& param);
}

void ProductionManagerWidget::rereleaseTransponder_clicked() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  void rereleaseTransponder_trigger(const StringDictionary& param);
}
