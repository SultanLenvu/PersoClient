#include "master_user_interface.h"
#include "global_environment.h"
#include "production_manager_gui_subkernel.h"
#include "programmator_user_interface.h"
#include "server_connection_gui_subkernel.h"
#include "sticker_printer_user_interface.h"
#include "widget_log_backend.h"

MasterUserInterface::MasterUserInterface(QWidget* parent) : QWidget(parent) {
  create();
  connectDependencies();
}

void MasterUserInterface::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  // Вкладки с всеми интерфейсами
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  // Интерфейс для взаимодействия с сервером персонализации
  createServerTab();

  // Интерфейс для взаимодействия с программатором
  createProgrammatorTab();

  // Интерфейс для взаимодействия с принтером стикером
  createStickerPrinterTab();

  // Виджеты для отображения логов
  createLogWidgets();

  // Подключаем внешние зависимости
  connectDepedencies();

  // Настройка пропорции между объектами на макете
  MainLayout->setStretch(0, 5);
  MainLayout->setStretch(1, 2);
}

void MasterUserInterface::createServerTab() {
  ServerTab = new QWidget();
  Tabs->addTab(ServerTab, "Сервер");

  ServerTabMainLayout = new QHBoxLayout();
  ServerTab->setLayout(ServerTabMainLayout);

  // Панель управления
  ServerControlPanel = new QGroupBox("Панель управления");
  ServerTabMainLayout->addWidget(ServerControlPanel);

  ServerControlPanelLayout = new QVBoxLayout();
  ServerControlPanel->setLayout(ServerControlPanelLayout);

  ServerConnectPushButton = new QPushButton(QString("Подключиться"));
  ServerControlPanelLayout->addWidget(ServerConnectPushButton);

  ServerDisconnectButton = new QPushButton(QString("Отключиться"));
  ServerControlPanelLayout->addWidget(ServerDisconnectButton);
  ServerEchoRequestButton = new QPushButton(QString("Отправить эхо-запрос"));
  ServerControlPanelLayout->addWidget(ServerEchoRequestButton);
  AuthorizePushButton = new QPushButton(QString("Авторизироваться"));
  ServerControlPanelLayout->addWidget(AuthorizePushButton);
  GetProductionLineDataButton =
      new QPushButton(QString("Запросить данные производственной линии"));
  ServerControlPanelLayout->addWidget(GetProductionLineDataButton);
  ServerControlPanelVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ServerControlPanelVS1);

  RequestBoxButton = new QPushButton(QString("Запросить бокс"));
  ServerControlPanelLayout->addWidget(RequestBoxButton);
  GetCurrentBoxDataButton =
      new QPushButton(QString("Запросить данные текущего бокса"));
  ServerControlPanelLayout->addWidget(GetCurrentBoxDataButton);
  RefundCurrentBoxButton = new QPushButton(QString("Вернуть бокс"));
  ServerControlPanelLayout->addWidget(RefundCurrentBoxButton);
  CompleteCurrentBoxButton = new QPushButton(QString("Завершить сборку бокса"));
  ServerControlPanelLayout->addWidget(CompleteCurrentBoxButton);
  ServerControlPanelVS2 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ServerControlPanelVS2);

  ReleaseTransponderButton = new QPushButton(QString("Выпустить транспондер"));
  ServerControlPanelLayout->addWidget(ReleaseTransponderButton);
  RereleaseTransponderButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  ServerControlPanelLayout->addWidget(RereleaseTransponderButton);
  RollbackTransponderPushButton =
      new QPushButton(QString("Откатить транспондер"));
  ServerControlPanelLayout->addWidget(RollbackTransponderPushButton);
  GetCurrentTransponderDataButton =
      new QPushButton(QString("Запросить данные текущего транспондера"));
  ServerControlPanelLayout->addWidget(GetCurrentTransponderDataButton);
  GetTransponderDataButton =
      new QPushButton(QString("Запросить данные транспондера"));
  ServerControlPanelLayout->addWidget(GetTransponderDataButton);
  ServerControlPanelVS3 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ServerControlPanelVS3);

  PrintBoxStickerButton =
      new QPushButton(QString("Распечатать стикер для бокса"));
  ServerControlPanelLayout->addWidget(PrintBoxStickerButton);
  PrintLastBoxStickerButton =
      new QPushButton(QString("Распечатать последний стикер для бокса"));
  ServerControlPanelLayout->addWidget(PrintLastBoxStickerButton);
  PrintPalletStickerButton =
      new QPushButton(QString("Распечатать стикер для паллеты"));
  ServerControlPanelLayout->addWidget(PrintPalletStickerButton);
  PrintLastPalletStickerButton =
      new QPushButton(QString("Распечатать последний стикер для паллеты"));
  ServerControlPanelLayout->addWidget(PrintLastPalletStickerButton);

  // Создаем представления
  createServerTabViews();

  // Настройка пропорции между объектами на макете
  ServerTabMainLayout->setStretch(0, 1);
  ServerTabMainLayout->setStretch(1, 2);
}

void MasterUserInterface::createServerTabViews() {
  ModelViewLayout = new QVBoxLayout();
  ServerTabMainLayout->addLayout(ModelViewLayout);

  // Данные производственной линии
  ProductionLineDataGroup = new QGroupBox("Данные производственной линии");
  ModelViewLayout->addWidget(ProductionLineDataGroup);

  ProductionLineDataLayout = new QVBoxLayout();
  ProductionLineDataGroup->setLayout(ProductionLineDataLayout);

  ProductionLineDataView = new QTableView();
  ProductionLineDataLayout->addWidget(ProductionLineDataView);

  // Дополнительный холст
  ModelViewSublayout = new QHBoxLayout();
  ModelViewLayout->addLayout(ModelViewSublayout);

  // Данные бокса
  BoxDataGroup = new QGroupBox("Данные бокса, находящегося в процессе сборки");
  ModelViewSublayout->addWidget(BoxDataGroup);

  BoxDataLayout = new QVBoxLayout();
  BoxDataGroup->setLayout(BoxDataLayout);

  BoxDataView = new QTableView();
  BoxDataLayout->addWidget(BoxDataView);

  // Данные транспондера
  TransponderDataGroup =
      new QGroupBox("Данные последнего выпущенного транспондера");
  ModelViewSublayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);
}

void MasterUserInterface::createProgrammatorTab() {
  ProgrammatorTab = new ProgrammatorUserInterface();
  Tabs->addTab(ProgrammatorTab, "Программатор");
}

void MasterUserInterface::createStickerPrinterTab() {
  StickerPrinterTab = new QWidget();
  Tabs->addTab(StickerPrinterTab, "Стикер принтер");
}

void MasterUserInterface::createLogWidgets() {
  // Виджеты для отображения логов
  GeneralLogGroup = new QGroupBox("Логи");
  MainLayout->addWidget(GeneralLogGroup);

  GeneralLogLayout = new QVBoxLayout();
  GeneralLogGroup->setLayout(GeneralLogLayout);

  GeneralLogs = new QPlainTextEdit();
  GeneralLogs->setObjectName(QString::fromUtf8("GeneralLogs"));
  GeneralLogs->setEnabled(true);
  GeneralLogs->setTabletTracking(true);
  GeneralLogs->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  GeneralLogs->setCenterOnScroll(false);
  GeneralLogLayout->addWidget(GeneralLogs);
}

void MasterUserInterface::connectDepedencies() {
  WidgetLogBackend* wlb =
      GlobalEnvironment::instance()->getObject<WidgetLogBackend>(
          "WidgetLogBackend");

  connect(wlb, &WidgetLogBackend::displayLog_signal, this,
          &MasterUserInterface::displayLog);
  connect(wlb, &WidgetLogBackend::clearLogDisplay_signal, this,
          &MasterUserInterface::clearLogDisplay);

  ServerConnectionGuiSubkernel* scgs =
      GlobalEnvironment::instance()->getObject<ServerConnectionGuiSubkernel>(
          "ServerConnectionGuiSubkernel");

  connect(ServerConnectPushButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::connect);
  connect(ServerDisconnectButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::disconnect);
  connect(ServerEchoRequestButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::echoRequest);
  connect(AuthorizePushButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::logOn);
  connect(GetProductionLineDataButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::getProductionLineData);

  connect(RequestBoxButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::requestBox);
  connect(GetCurrentBoxDataButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::getCurrentBoxData);
  connect(RefundCurrentBoxButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::refundCurrentBox);
  connect(CompleteCurrentBoxButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::completeCurrentBox);

  connect(ReleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::releaseTransponder);
  connect(RereleaseTransponderButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::rereleaseTransponder);
  connect(RollbackTransponderPushButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::rollbackTransponder);
  connect(GetCurrentTransponderDataButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::getCurrentTransponderData);
  connect(GetTransponderDataButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::getTransponderData);

  connect(PrintBoxStickerButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::printBoxSticker);
  connect(PrintLastBoxStickerButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::printLastBoxSticker);
  connect(PrintPalletStickerButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::printPalletSticker);
  connect(PrintLastPalletStickerButton, &QPushButton::clicked, this,
          &ProductionManagerGuiSubkernel::printLastPalletSticker);

  ProductionManagerGuiSubkernel* pmgs =
      GlobalEnvironment::instance()->getObject<ProductionManagerGuiSubkernel>(
          "ProductionManagerGuiSubkernel");

  // Связывание моделей и представлений
  ProductionLineDataView->setModel(&pmgs->productionLineModel());
  TransponderDataView->setModel(&pmgs->transponderModel());
  BoxDataView->setModel(&pmgs->boxModel());
}

void MasterUserInterface::displayLog(const QString& log) {
  if (GeneralLogs->toPlainText().size() > 500000)
    GeneralLogs->clear();

  GeneralLogs->appendPlainText(log);
}

void MasterUserInterface::clearLogDisplay() {
  GeneralLogs->clear();
}
