#include "master_gui.h"

MasterGUI::MasterGUI(QWidget* parent) : AbstractGui(parent, Master) {
  // Вкладки с всеми интерфейсами
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  // Интерфейс для взаимодействия с сервером персонализации
  createServerTab();

  // Интерфейс для взаимодействия с программатором
  createProgrammatorTab();

  // Интерфейс для взаимодействия с принтером стикером
  createStickerPrinterTab();

  // Интерфейс для изменения настроек
  createSettingsTab();

  // Виджеты для отображения логов
  createLogWidgets();

  // Настройка пропорции между объектами на макете
  MainLayout->setStretch(0, 3);
  MainLayout->setStretch(1, 2);
}

MasterGUI::~MasterGUI() {}

void MasterGUI::update() {
  TransponderDataView->resizeColumnsToContents();
  TransponderDataView->update();
}

void MasterGUI::displayLogData(const QString& log) {
  if (GeneralLogs->toPlainText().count() > 500000)
    GeneralLogs->clear();

  GeneralLogs->appendPlainText(log);
}

void MasterGUI::clearLogDataDisplay() {
  GeneralLogs->clear();
}

void MasterGUI::createServerTab() {
  ServerTab = new QWidget();
  Tabs->addTab(ServerTab, "Сервер");

  ServerTabMainLayout = new QHBoxLayout();
  ServerTab->setLayout(ServerTabMainLayout);

  // Панель управления
  ServerControlPanel = new QGroupBox("Панель управления");
  ServerTabMainLayout->addWidget(ServerControlPanel);

  ServerControlPanelLayout = new QVBoxLayout();
  ServerControlPanel->setLayout(ServerControlPanelLayout);

  ServerConnectPushButton =
      new QPushButton(QString("Подключиться к серверу персонализации"));
  ServerControlPanelLayout->addWidget(ServerConnectPushButton);

  ServerDisconnectButton =
      new QPushButton(QString("Отключиться от сервера персонализации"));
  ServerControlPanelLayout->addWidget(ServerDisconnectButton);

  ServerEchoRequestButton =
      new QPushButton(QString("Отправить эхо-запрос"));
  ServerControlPanelLayout->addWidget(ServerEchoRequestButton);

  ServerControlPanelVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ServerControlPanelVS1);

  AuthorizePushButton = new QPushButton(QString("Авторизироваться"));
  ServerControlPanelLayout->addWidget(AuthorizePushButton);
  ReleaseTransponderButton = new QPushButton(QString("Выпустить транспондер"));
  ServerControlPanelLayout->addWidget(ReleaseTransponderButton);
  RereleaseTransponderButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  ServerControlPanelLayout->addWidget(RereleaseTransponderButton);
  RollbackProductionLinePushButton =
      new QPushButton(QString("Откатить производственную линию"));
  ServerControlPanelLayout->addWidget(RollbackProductionLinePushButton);
  ServerControlPanelVS2 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ServerControlPanelVS2);

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

  // Представление данных о транспондере
  TransponderDataGroup = new QGroupBox("Данные транспондера");
  ServerTabMainLayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);

  // Настройка пропорции между объектами на макете
  ServerTabMainLayout->setStretch(0, 1);
  ServerTabMainLayout->setStretch(1, 3);
}

void MasterGUI::createProgrammatorTab() {
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
  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ProgrammatorControlPanelLayout->addItem(ButtonVerticalSpacer);

  UnlockDeviceButton = new QPushButton(QString("Разблокировать память"));
  ProgrammatorControlPanelLayout->addWidget(UnlockDeviceButton);
  LockDeviceButton = new QPushButton(QString("Заблокировать память"));
  ProgrammatorControlPanelLayout->addWidget(LockDeviceButton);
}

void MasterGUI::createStickerPrinterTab() {
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

void MasterGUI::createSettingsTab() {
  SettingsTab = new QWidget();
  Tabs->addTab(SettingsTab, "Настройки");
  // Загружаем настройки приложения
  QSettings settings;

  // Главный макет меню настроек
  SettingsMainLayout = new QHBoxLayout();
  SettingsTab->setLayout(SettingsMainLayout);

  SettingsMainSubLayout = new QVBoxLayout();
  SettingsMainLayout->addLayout(SettingsMainSubLayout);

  // Система логгирования
  LogSystemSettingsGroupBox = new QGroupBox(QString("Система логгирования"));
  SettingsMainSubLayout->addWidget(LogSystemSettingsGroupBox);

  LogSystemSettingsMainLayout = new QGridLayout();
  LogSystemSettingsGroupBox->setLayout(LogSystemSettingsMainLayout);

  LogSystemGlobalEnableLabel = new QLabel("Глобальное логгирование вкл/выкл");
  LogSystemSettingsMainLayout->addWidget(LogSystemGlobalEnableLabel, 0, 0, 1,
                                         1);
  LogSystemGlobalEnableCheckBox = new QCheckBox();
  LogSystemGlobalEnableCheckBox->setCheckState(
      settings.value("log_system/global_enable").toBool() ? Qt::Checked
                                                          : Qt::Unchecked);
  LogSystemSettingsMainLayout->addWidget(LogSystemGlobalEnableCheckBox, 0, 1, 1,
                                         1);
  LogSystemExtendedEnableLabel =
      new QLabel("Расширенное логгирование вкл/выкл");
  LogSystemSettingsMainLayout->addWidget(LogSystemExtendedEnableLabel, 1, 0, 1,
                                         1);
  LogSystemExtendedEnableCheckBox = new QCheckBox();
  LogSystemExtendedEnableCheckBox->setCheckState(
      settings.value("log_system/Extended_enable").toBool() ? Qt::Checked
                                                            : Qt::Unchecked);
  LogSystemSettingsMainLayout->addWidget(LogSystemExtendedEnableCheckBox, 1, 1,
                                         1, 1);

  // Сеть
  PersoSettingsGroupBox = new QGroupBox(QString("Сетевые настройки"));
  SettingsMainSubLayout->addWidget(PersoSettingsGroupBox);

  PersoSettingsMainLayout = new QGridLayout();
  PersoSettingsGroupBox->setLayout(PersoSettingsMainLayout);

  PersoServerIpAddressLabel =
      new QLabel("IP адрес или URL сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLabel, 0, 0, 1, 1);
  PersoServerIpAddressLineEdit =
      new QLineEdit(settings.value("perso_client/server_ip").toString());
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLineEdit, 0, 1, 1, 1);
  PersoServerPortLabel = new QLabel("Порт сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerPortLabel, 1, 0, 1, 1);
  PersoServerPortLineEdit =
      new QLineEdit(settings.value("perso_client/server_port").toString());
  PersoSettingsMainLayout->addWidget(PersoServerPortLineEdit, 1, 1, 1, 1);

  // Настройки программатора
  ProgrammerSettingsGroupBox = new QGroupBox(QString("Программатор"));
  SettingsMainSubLayout->addWidget(ProgrammerSettingsGroupBox);

  ProgrammerSettingsMainLayout = new QGridLayout();
  ProgrammerSettingsGroupBox->setLayout(ProgrammerSettingsMainLayout);

  ProgrammerExeFilePathLabel = new QLabel("Путь к JLink.exe");
  ProgrammerSettingsMainLayout->addWidget(ProgrammerExeFilePathLabel, 0, 0, 1,
                                          1);
  ProgrammerExeFilePathLineEdit = new QLineEdit(
      settings.value("jlink_exe_programmer/exe_file_path").toString());
  ProgrammerSettingsMainLayout->addWidget(ProgrammerExeFilePathLineEdit, 0, 1,
                                          1, 1);
  ProgrammerExeFilePathPushButton = new QPushButton("Обзор");
  ProgrammerSettingsMainLayout->addWidget(ProgrammerExeFilePathPushButton, 0, 2,
                                          1, 1);
  connect(ProgrammerExeFilePathPushButton, &QPushButton::clicked, this,
          &MasterGUI::on_ProgrammerExeFilePathPushButton_slot);
  ProgrammerSpeedLabel = new QLabel("Скорость работы (кГц)");
  ProgrammerSettingsMainLayout->addWidget(ProgrammerSpeedLabel, 1, 0, 1, 1);
  ProgrammerSpeedLineEdit =
      new QLineEdit(settings.value("jlink_exe_programmer/speed").toString());
  ProgrammerSettingsMainLayout->addWidget(ProgrammerSpeedLineEdit, 1, 1, 1, 2);

  // Настройки принтера
  StickerPrinterSettingsGroupBox = new QGroupBox(QString("Стикер-принтер"));
  SettingsMainSubLayout->addWidget(StickerPrinterSettingsGroupBox);

  StickerPrinterSettingsMainLayout = new QGridLayout();
  StickerPrinterSettingsGroupBox->setLayout(StickerPrinterSettingsMainLayout);

  StickerPrinterLibPathLabel = new QLabel("Путь к библиотеке");
  StickerPrinterSettingsMainLayout->addWidget(StickerPrinterLibPathLabel, 0, 0,
                                              1, 1);
  StickerPrinterLibPathLineEdit =
      new QLineEdit(settings.value("te310_printer/library_path").toString());
  StickerPrinterSettingsMainLayout->addWidget(StickerPrinterLibPathLineEdit, 0,
                                              1, 1, 1);
  StickerPrinterLibPathPushButton = new QPushButton("Обзор");
  StickerPrinterSettingsMainLayout->addWidget(StickerPrinterLibPathPushButton,
                                              0, 2, 1, 1);
  connect(StickerPrinterLibPathPushButton, &QPushButton::clicked, this,
          &MasterGUI::on_StickerPrinterLibPathPushButton_slot);

  // Кнопка сохранения настроек
  ApplySettingsPushButton = new QPushButton("Применить изменения");
  SettingsMainSubLayout->addWidget(ApplySettingsPushButton);

  // Сжимаем позиционирование
  SettingsVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  SettingsMainSubLayout->addItem(SettingsVS1);
  SettingsHS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  SettingsMainLayout->addItem(SettingsHS1);
}

void MasterGUI::createLogWidgets() {
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

void MasterGUI::on_ProgrammerExeFilePathPushButton_slot() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "", "*.exe");
  ProgrammerExeFilePathLineEdit->setText(filePath);
}

void MasterGUI::on_StickerPrinterLibPathPushButton_slot() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "", "*.dll");
  StickerPrinterLibPathLineEdit->setText(filePath);
}
