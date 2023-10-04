#include "master_gui.h"

MasterGUI::MasterGUI(QWidget* parent) : GUI(parent, Master) {}

void MasterGUI::create() {
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

void MasterGUI::update() {
  TransponderInfoView->resizeColumnsToContents();
  TransponderInfoView->update();
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

  PersoServerConnectPushButton =
      new QPushButton(QString("Подключиться к серверу персонализации"));
  ServerControlPanelLayout->addWidget(PersoServerConnectPushButton);

  PersoServerDisconnectButton =
      new QPushButton(QString("Отключиться от сервера персонализации"));
  ServerControlPanelLayout->addWidget(PersoServerDisconnectButton);

  PersoServerEchoRequestButton = new QPushButton(QString("Отправить эхо-запрос"));
  ServerControlPanelLayout->addWidget(PersoServerEchoRequestButton);

  ButtonVerticalSpacer1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ButtonVerticalSpacer1);

  MasterAuthorizePushButton = new QPushButton(QString("Авторизироваться"));
  ServerControlPanelLayout->addWidget(MasterAuthorizePushButton);

  LoadTransponderFirmwareButton =
      new QPushButton(QString("Выпуск транспондера"));
  ServerControlPanelLayout->addWidget(LoadTransponderFirmwareButton);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпуск транспондера"));
  ServerControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  // Представление данных о транспондере
  TransponderInfoGroup = new QGroupBox("Данные о выпущенном транспондере");
  ServerTabMainLayout->addWidget(TransponderInfoGroup);

  TransponderInfoLayout = new QVBoxLayout();
  TransponderInfoGroup->setLayout(TransponderInfoLayout);

  TransponderInfoView = new QTableView();
  TransponderInfoLayout->addWidget(TransponderInfoView);

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

  // Общие
  GeneralSettingsGroupBox = new QGroupBox(QString("Общие"));
  SettingsMainSubLayout->addWidget(GeneralSettingsGroupBox);

  GeneralSettingsMainLayout = new QGridLayout();
  GeneralSettingsGroupBox->setLayout(GeneralSettingsMainLayout);

  ExtendedLoggingEnableLabel = new QLabel("Расширенное логгирование");
  GeneralSettingsMainLayout->addWidget(ExtendedLoggingEnableLabel, 0, 0, 1, 1);
  ExtendedLoggingEnableCheckBox = new QCheckBox();
  ExtendedLoggingEnableCheckBox->setCheckState(
      settings.value("General/ExtendedLoggingEnable").toBool() ? Qt::Checked
                                                               : Qt::Unchecked);
  GeneralSettingsMainLayout->addWidget(ExtendedLoggingEnableCheckBox, 0, 1, 1,
                                       1);

  // Сеть
  PersoSettingsGroupBox = new QGroupBox(QString("Сетевые настройки"));
  SettingsMainSubLayout->addWidget(PersoSettingsGroupBox);

  PersoSettingsMainLayout = new QGridLayout();
  PersoSettingsGroupBox->setLayout(PersoSettingsMainLayout);

  PersoServerIpAddressLabel =
      new QLabel("IP адрес или URL сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLabel, 0, 0, 1, 1);
  PersoServerIpAddressLineEdit = new QLineEdit(
      settings.value("Personalization/ServerIpAddress").toString());
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLineEdit, 0, 1, 1, 1);
  PersoServerPortLabel = new QLabel("Порт сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerPortLabel, 1, 0, 1, 1);
  PersoServerPortLineEdit =
      new QLineEdit(settings.value("Personalization/ServerPort").toString());
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
      settings.value("JLinkExeProgrammer/ExeFile/Path").toString());
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
      new QLineEdit(settings.value("JLinkExeProgrammer/Speed").toString());
  ProgrammerSettingsMainLayout->addWidget(ProgrammerSpeedLineEdit, 1, 1, 1, 2);

  // Настройки принтера
  PrinterSettingsGroupBox = new QGroupBox(QString("Стикер-принтер"));
  SettingsMainSubLayout->addWidget(PrinterSettingsGroupBox);

  PrinterSettingsMainLayout = new QGridLayout();
  PrinterSettingsGroupBox->setLayout(PrinterSettingsMainLayout);

  PrinterLibPathLabel = new QLabel("Путь к библиотеке");
  PrinterSettingsMainLayout->addWidget(PrinterLibPathLabel, 0, 0, 1, 1);
  PrinterLibPathLineEdit =
      new QLineEdit(settings.value("StickerPrinter/DLL/Path").toString());
  PrinterSettingsMainLayout->addWidget(PrinterLibPathLineEdit, 0, 1, 1, 1);
  PrinterLibPathPushButton = new QPushButton("Обзор");
  PrinterSettingsMainLayout->addWidget(PrinterLibPathPushButton, 0, 2, 1, 1);
  connect(PrinterLibPathPushButton, &QPushButton::clicked, this,
          &MasterGUI::on_PrinterLibPathPushButton_slot);

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

void MasterGUI::on_PrinterLibPathPushButton_slot() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "", "*.dll");
  PrinterLibPathLineEdit->setText(filePath);
}
