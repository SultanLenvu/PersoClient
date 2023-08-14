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
}

void MasterGUI::update() {}

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
  PersoServerConnectPushButton->setFont(QFont("Arial", 14, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerConnectPushButton);

  PersoServerDisconnectButton =
      new QPushButton(QString("Отключиться от сервера персонализации"));
  PersoServerDisconnectButton->setFont(QFont("Arial", 14, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerDisconnectButton);

  PersoServerSendEchoButton = new QPushButton(QString("Отправить эхо-запрос"));
  PersoServerSendEchoButton->setFont(QFont("Arial", 14, QFont::Bold));
  ServerControlPanelLayout->addWidget(PersoServerSendEchoButton);

  ButtonVerticalSpacer1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ServerControlPanelLayout->addItem(ButtonVerticalSpacer1);
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

  AutoProgramDeviceButton =
      new QPushButton(QString("Автоматическая зарузка прошивки"));
  AutoProgramDeviceButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(AutoProgramDeviceButton);

  ManualProgramDeviceButton =
      new QPushButton(QString("Ручная зарузка прошивки"));
  ManualProgramDeviceButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ManualProgramDeviceButton);

  ReadDeviceFirmwareButton = new QPushButton(QString("Считать прошивку"));
  ReadDeviceFirmwareButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceFirmwareButton);

  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  EraseDeviceButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(EraseDeviceButton);

  ProgramDeviceUserDataButton =
      new QPushButton(QString("Загрузить  пользовательские данные"));
  ProgramDeviceUserDataButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ProgramDeviceUserDataButton);

  ReadDeviceUserDataButton =
      new QPushButton(QString("Считать  пользовательские данные"));
  ReadDeviceUserDataButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(ReadDeviceUserDataButton);

  UnlockDeviceButton = new QPushButton(QString("Разблокировать память"));
  UnlockDeviceButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(UnlockDeviceButton);

  LockDeviceButton = new QPushButton(QString("Заблокировать память"));
  LockDeviceButton->setFont(QFont("Arial", 14, QFont::Bold));
  ProgrammatorControlPanelLayout->addWidget(LockDeviceButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ProgrammatorControlPanelLayout->addItem(ButtonVerticalSpacer);
}

void MasterGUI::createSettingsTab() {
  SettingsTab = new QWidget();
  Tabs->addTab(SettingsTab, "Настройки");

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
  PersoSettingsMainLayout->addWidget(UsePersoServerCheckBox, 0, 1, 1, 1);

  PersoServerIpAddressLabel =
      new QLabel("IP адрес или URL сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLabel, 1, 0, 1, 1);

  PersoServerIpAddressLineEdit = new QLineEdit(PERSO_SERVER_DEFAULT_IP);
  PersoSettingsMainLayout->addWidget(PersoServerIpAddressLineEdit, 1, 1, 1, 1);

  PersoServerPortLabel = new QLabel("Порт сервера персонализации");
  PersoSettingsMainLayout->addWidget(PersoServerPortLabel, 2, 0, 1, 1);

  PersoServerPortLineEdit =
      new QLineEdit(QString::number(PERSO_SERVER_DEFAULT_PORT));
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

void MasterGUI::displayLogData(const QString &log) {
  if (GeneralLogs->toPlainText().count() > 100000)
    GeneralLogs->clear();

  GeneralLogs->appendPlainText(log);
}
