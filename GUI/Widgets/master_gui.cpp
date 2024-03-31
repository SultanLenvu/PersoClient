#include "master_gui.h"
#include "global_environment.h"
#include "widget_log_backend.h"

MasterGui::MasterGui(QWidget* parent) : AbstractGui(parent) {
  create();
}

void MasterGui::create() {
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

void MasterGui::createServerTab() {
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

void MasterGui::createServerTabViews() {
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

void MasterGui::createProgrammatorTab() {
  ProgrammatorTab = new QWidget();
  Tabs->addTab(ProgrammatorTab, "Программатор");

  ProgrammatorTabMainLayout = new QHBoxLayout();
  ProgrammatorTab->setLayout(ProgrammatorTabMainLayout);

  // Панель управления
  ProgrammatorControlPanel = new QGroupBox("Панель управления");
  ProgrammatorTabMainLayout->addWidget(ProgrammatorControlPanel);

  ProgrammatorControlPanelLayout = new QVBoxLayout();
  ProgrammatorControlPanel->setLayout(ProgrammatorControlPanelLayout);

  ProgramDeviceButton = new QPushButton(QString("Ручная зарузка прошивки"));
  ProgrammatorControlPanelLayout->addWidget(ProgramDeviceButton);
  ReadDeviceFirmwareButton = new QPushButton(QString("Считать прошивку"));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceFirmwareButton);
  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  ProgrammatorControlPanelLayout->addWidget(EraseDeviceButton);
  ProgramDeviceUserDataButton =
      new QPushButton(QString("Загрузить  пользовательские данные"));
  ProgrammatorControlPanelLayout->addWidget(ProgramDeviceUserDataButton);
  ReadDeviceUserDataButton =
      new QPushButton(QString("Считать  пользовательские данные"));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceUserDataButton);
  ReadDeviceUcidButton = new QPushButton(QString("Считать  UCID"));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceUcidButton);

  ButtonVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ProgrammatorControlPanelLayout->addItem(ButtonVS1);

  unlockMemoryButton = new QPushButton(QString("Разблокировать память"));
  ProgrammatorControlPanelLayout->addWidget(unlockMemoryButton);
  lockMemoryButton = new QPushButton(QString("Заблокировать память"));
  ProgrammatorControlPanelLayout->addWidget(lockMemoryButton);
}

void MasterGui::createStickerPrinterTab() {
  StickerPrinterTab = new QWidget();
  Tabs->addTab(StickerPrinterTab, "Стикер принтер");
  // Загружаем настройки приложения
  QSettings settings;

  // Главный макет меню настроек
  StickerPrinterTabMainLayout = new QHBoxLayout();
  StickerPrinterTab->setLayout(StickerPrinterTabMainLayout);

  StickerPrinterControlPanel = new QGroupBox(QString("Панель управления"));
  StickerPrinterTabMainLayout->addWidget(StickerPrinterControlPanel);
  StickerPrinterControlPanelLayout = new QVBoxLayout();
  StickerPrinterControlPanel->setLayout(StickerPrinterControlPanelLayout);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  StickerPrinterControlPanelLayout->addWidget(
      PrintLastTransponderStickerButton);
  PrintCustomTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  StickerPrinterControlPanelLayout->addWidget(
      PrintCustomTransponderStickerButton);
  StickerPrinterControlPanelVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  StickerPrinterControlPanelLayout->addItem(StickerPrinterControlPanelVS1);
  ExecuteStickerPrinterCommandScriptButton =
      new QPushButton(QString("Выполнить командный скрипт"));
  StickerPrinterControlPanelLayout->addWidget(
      ExecuteStickerPrinterCommandScriptButton);

  StickerPrinterCommandSriptGroup = new QGroupBox(QString("Командный скрипт"));
  StickerPrinterTabMainLayout->addWidget(StickerPrinterCommandSriptGroup);
  StickerPrinterCommandSriptLayout = new QVBoxLayout();
  StickerPrinterCommandSriptGroup->setLayout(StickerPrinterCommandSriptLayout);
  StickerPrinterCommandSriptTextEdit = new QPlainTextEdit();
  StickerPrinterCommandSriptLayout->addWidget(
      StickerPrinterCommandSriptTextEdit);
}

void MasterGui::createLogWidgets() {
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

void MasterGui::connectDepedencies() {
  WidgetLogBackend* ls = dynamic_cast<WidgetLogBackend*>(
      GlobalEnvironment::instance()->getObject("WidgetLogBackend"));
  assert(ls != nullptr);

  connect(ls, &WidgetLogBackend::displayLog_signal, this,
          &MasterGui::displayLog);
  connect(ls, &WidgetLogBackend::clearLogDisplay_signal, this,
          &MasterGui::clearLogDisplay);
}

void MasterGui::displayLog(const QString& log) {
  if (GeneralLogs->toPlainText().size() > 500000)
    GeneralLogs->clear();

  GeneralLogs->appendPlainText(log);
}

void MasterGui::clearLogDisplay() {
  GeneralLogs->clear();
}
