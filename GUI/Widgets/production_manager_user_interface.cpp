#include "production_manager_user_interface.h"
#include "global_environment.h"
#include "production_manager_gui_subkernel.h"
#include "production_unit_context.h"
#include "server_commands_widget.h"

ProductionManagerUserInterface::ProductionManagerUserInterface(QWidget* parent)
    : AbstractUserInterface{parent} {
  createWidgets();
  connectDependencies();
}

void ProductionManagerUserInterface::displayFirmware(
    const QByteArray& firmware) {
  FirmwareView->setPlainText(firmware);
}

void ProductionManagerUserInterface::createWidgets() {
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

void ProductionManagerUserInterface::createControlPanel() {
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

void ProductionManagerUserInterface::createCommandsWidget() {
  CommandsWidget = new ServerCommandsWidget();
  SubLayout->addWidget(CommandsWidget);
}

void ProductionManagerUserInterface::createInitGroup() {
  InitGroup = new QGroupBox("Авторизация");
  ControlPanelLayout->addWidget(InitGroup);

  InitGroupLayout = new QVBoxLayout();
  InitGroup->setLayout(InitGroupLayout);

  LogOnPushButton = new QPushButton("Авторизироваться");
  InitGroupLayout->addWidget(LogOnPushButton);
  LogOutPushButton = new QPushButton("Выйти");
  InitGroupLayout->addWidget(LogOutPushButton);
}

void ProductionManagerUserInterface::createBoxGroup() {
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

void ProductionManagerUserInterface::createTransponderGroup() {
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

void ProductionManagerUserInterface::createPrintingGroup() {}

void ProductionManagerUserInterface::createDataDisplayPanel() {
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

void ProductionManagerUserInterface::connectDependencies() {
  ProductionManagerGuiSubkernel* augs =
      GlobalEnvironment::instance()->getObject<ProductionManagerGuiSubkernel>(
          "ProductionManagerGuiSubkernel");

  void (ProductionManagerGuiSubkernel::*mptr)(void) =
      &ProductionManagerGuiSubkernel::logOn;
  QObject::connect(LogOnPushButton, &QPushButton::clicked, augs, mptr);
  QObject::connect(LogOutPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::logOut);

  QObject::connect(RequestBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::requestBox);
  QObject::connect(RefundBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::refundCurrentBox);
  QObject::connect(CompleteCurrentBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::completeCurrentBox);

  QObject::connect(ReleaseTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::releaseTransponder);
  QObject::connect(RereleaseTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::rereleaseTransponder);
  QObject::connect(RollbackTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::rollbackTransponder);

  ProductionLineDataView->setModel(&augs->productionLineModel());
  BoxDataView->setModel(&augs->boxModel());
  TransponderDataView->setModel(&augs->transponderModel());

  ProductionUnitContext* puc =
      GlobalEnvironment::instance()->getObject<ProductionUnitContext>(
          "ProductionUnitContext");

  QObject::connect(puc, &ProductionUnitContext::firmwareChanged, this,
                   &ProductionManagerUserInterface::displayFirmware);
}
