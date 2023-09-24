#include "master_gui.h"

MasterGUI::MasterGUI(QWidget* parent) : GUI(parent, Master) {}

void MasterGUI::create() {
  // Вкладки с всеми интерфейсами
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  // Интерфейс для взаимодействия с сервером персонализации
  createServerTab();

  // Интерфейс для взаиомдейтсвия с программатором
  createProgrammatorTab();

  // Интерфейс для изменения настроек
  createSettingsTab();

  // Виджеты для отображения логов
  createLogWidgets();

  // Настройка пропорции между объектами на макете
  MainLayout->setStretch(0, 3);
  MainLayout->setStretch(1, 2);
}

void MasterGUI::update() {
  TransponderSeedView->resizeColumnsToContents();
  TransponderSeedView->update();
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
  PersoServerConnectPushButton->setFont(QFont("Arial", 12, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerConnectPushButton);

  PersoServerDisconnectButton =
      new QPushButton(QString("Отключиться от сервера персонализации"));
  PersoServerDisconnectButton->setFont(QFont("Arial", 12, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerDisconnectButton);

  PersoServerEchoRequestButton = new QPushButton(QString("Отправить эхо-запрос"));
  PersoServerEchoRequestButton->setFont(QFont("Arial", 12, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerEchoRequestButton);

  ButtonVerticalSpacer1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ButtonVerticalSpacer1);

  LoadTransponderFirmwareButton =
      new QPushButton(QString("Выпуск транспондера"));
  LoadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ServerControlPanelLayout->addWidget(LoadTransponderFirmwareButton);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпуск транспондера"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ServerControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  // Представление данных о транспондере
  TransponderSeedGroup = new QGroupBox("Данные о выпущенном транспондере");
  ServerTabMainLayout->addWidget(TransponderSeedGroup);

  TransponderSeedLayout = new QVBoxLayout();
  TransponderSeedGroup->setLayout(TransponderSeedLayout);

  TransponderSeedView = new QTableView();
  TransponderSeedLayout->addWidget(TransponderSeedView);

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
  ProgramDeviceButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ProgramDeviceButton);
  ReadDeviceFirmwareButton = new QPushButton(QString("Считать прошивку"));
  ReadDeviceFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceFirmwareButton);
  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  EraseDeviceButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(EraseDeviceButton);
  ProgramDeviceUserDataButton =
      new QPushButton(QString("Загрузить  пользовательские данные"));
  ProgramDeviceUserDataButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ProgramDeviceUserDataButton);
  ReadDeviceUserDataButton =
      new QPushButton(QString("Считать  пользовательские данные"));
  ReadDeviceUserDataButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceUserDataButton);
  UnlockDeviceButton = new QPushButton(QString("Разблокировать память"));
  UnlockDeviceButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(UnlockDeviceButton);
  LockDeviceButton = new QPushButton(QString("Заблокировать память"));
  LockDeviceButton->setFont(QFont("Arial", 12, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(LockDeviceButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ProgrammatorControlPanelLayout->addItem(ButtonVerticalSpacer);
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

  // Настройки персонализации
  QGroupBox* PersoSettingsGroupBox =
      new QGroupBox(QString("Настройки персонализации"));
  SettingsMainSubLayout->addWidget(PersoSettingsGroupBox);

  PersoSettingsMainLayout = new QGridLayout();
  PersoSettingsGroupBox->setLayout(PersoSettingsMainLayout);

  UsePersoServerLabel = new QLabel("Использование сервера персонализации");
  PersoSettingsMainLayout->addWidget(UsePersoServerLabel, 0, 0, 1, 1);

  UsePersoServerCheckBox = new QCheckBox("Вкл/выкл");
  UsePersoServerCheckBox->setChecked(
      settings.value("Personalization/UseServerOption").toBool());
  PersoSettingsMainLayout->addWidget(UsePersoServerCheckBox, 0, 1, 1, 1);

  PersoServerIpAddressLabel =
      new QLabel("IP адрес или URL сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLabel, 1, 0, 1, 1);

  PersoServerIpAddressLineEdit = new QLineEdit(
      settings.value("Personalization/ServerIpAddress").toString());
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLineEdit, 1, 1, 1, 1);

  PersoServerPortLabel = new QLabel("Порт сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerPortLabel, 2, 0, 1, 1);

  PersoServerPortLineEdit =
      new QLineEdit(settings.value("Personalization/ServerPort").toString());
  PersoSettingsMainLayout->addWidget(PersoServerPortLineEdit, 2, 1, 1, 1);

  // Кнопка сохранения настроек
  ApplySettingsPushButton = new QPushButton("Применить изменения");
  SettingsMainSubLayout->addWidget(ApplySettingsPushButton);

  // Сжимаем позиционирование
  SettingsVerticalSpacer1 =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  SettingsMainSubLayout->addItem(SettingsVerticalSpacer1);
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
