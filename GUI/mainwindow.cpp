#include "mainwindow.h"

MainWindow::MainWindow() {
  setObjectName("MainWindow");

  // Загружаем настройки приложения
  loadSettings();

  // Интерфейс пока не создан
  CurrentGUI = nullptr;

  // Система для взаимодействия с пользователем
  UserInteraction = new UserInteractionSystem(this, this);
  connect(this, &MainWindow::requestMasterPasswordFromUser, UserInteraction,
          &UserInteractionSystem::getMasterPassword);
  connect(this, &MainWindow::notifyUser, UserInteraction,
          &UserInteractionSystem::generateMessage);
  connect(this, &MainWindow::notifyUserAboutError, UserInteraction,
          &UserInteractionSystem::generateErrorMessage);

  // Менеджер для взаимодействия с программатором
  Manager = new ClientManager(this);
  connect(Manager, &ClientManager::notifyUser, UserInteraction,
          &UserInteractionSystem::generateMessage);
  connect(Manager, &ClientManager::notifyUserAboutError, UserInteraction,
          &UserInteractionSystem::generateErrorMessage);

  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем логгер
  Logger = new LogSystem(this);
  connect(Manager, &ClientManager::logging, Logger, &LogSystem::generateLog);

  // Создаем графический интерфейс
  createProductionInterface();
}

MainWindow::~MainWindow() {}

void MainWindow::on_AutoProgramDeviceButton_slot() {
  Logger->clear();

  Manager->performFirmwareLoading(DEFAULT_FIRMWARE_FILE_PATH, true);
}

void MainWindow::on_ManualProgramDeviceButton_slot() {
  Logger->clear();

  Manager->performFirmwareLoading(
      QFileDialog::getOpenFileName(nullptr, "Выберите файл", "",
                                   "Все файлы (*.*)"),
      false);
}

void MainWindow::on_ReadDeviceFirmwareButton_slot() {
  Logger->clear();

  Manager->performFirmwareReading();
}

void MainWindow::on_EraseDeviceButton_slot() {
  Logger->clear();

  Manager->performFirmwareErasing();
}

void MainWindow::on_ProgramDeviceUserDataButton_slot() {
  Logger->clear();

  Manager->performUserDataLoading(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)"));
}

void MainWindow::on_ReadDeviceUserDataButton_slot() {
  Logger->clear();

  Manager->performUserDataReading();
}

void MainWindow::on_UnlockDeviceButton_slot() {
  Logger->clear();

  Manager->performDeviceUnlock();
}

void MainWindow::on_LockDeviceButton_slot() {
  Logger->clear();

  Manager->performDeviceLock();
}

void MainWindow::on_ApplySettingsPushButton_slot() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);

  Logger->clear();

  // Проверка пользовательского ввода
  if (!checkNewSettings()) {
    emit notifyUserAboutError("Введены некорректные данные для настроек. ");
    return;
  }

  // Считывание пользовательского ввода
  Settings->setValue("Personalization/UseServerOption",
                     gui->UsePersoServerCheckBox->isChecked());
  Settings->setValue("Personalization/ServerIpAddress",
                     gui->PersoServerIpAddressLineEdit->text());
  Settings->setValue("Personalization/ServerPort",
                     gui->PersoServerPortLineEdit->text().toInt());

  // Применение новых настроек
  Manager->applySettings();

  emit notifyUser("Новые настройки успешно применены. ");
}

void MainWindow::on_PersoServerConnectPushButton_slot() {
  Logger->clear();

  Manager->performServerConnecting();
}

void MainWindow::on_PersoServerDisconnectButton_slot() {
  Logger->clear();

  Manager->performServerDisconnecting();
}

void MainWindow::on_PersoServerSendEchoButton_slot() {
  Logger->clear();

  Manager->performServerEchoRequest();
}

void MainWindow::on_MasterInterfaceRequestAct_slot() {
  createMasterInterface();
}

void MainWindow::on_ProductionInterfaceRequestAct_slot() {
  createProductionInterface();

  Logger->setEnable(false);
}

void MainWindow::loadSettings() {
  QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(PROGRAM_NAME);

  Settings = new QSettings(this);
}

bool MainWindow::checkNewSettings() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);

  QHostAddress IP = QHostAddress(gui->PersoServerIpAddressLineEdit->text());

  if (IP.isNull()) {
    return false;
  }

  int32_t port = gui->PersoServerPortLineEdit->text().toInt();

  if ((port > IP_PORT_MAX_VALUE) || (port < IP_PORT_MIN_VALUE)) {
    return false;
  }

  return true;
}

void MainWindow::createMasterInterface() {
  QString pass;
  emit requestMasterPasswordFromUser(pass);

  if (pass == QString(MASTER_ACCESS_PASSWORD)) {
    // Создаем интерфейс
    delete CurrentGUI;
    CurrentGUI = new MasterGUI(this);
    CurrentGUI->create();
    setCentralWidget(CurrentGUI);

    // Подключаем интерфейс
    connectMasterInterface();

    // Создаем верхнее меню
    createTopMenu();

    // Включаем систему логгирования
    Logger->setEnable(true);
  } else
    emit notifyUserAboutError("Неверный пароль");
}

void MainWindow::connectMasterInterface() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);

  // Подключение виджетов
  connect(gui->PersoServerConnectPushButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerConnectPushButton_slot);
  connect(gui->PersoServerDisconnectButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerDisconnectButton_slot);
  connect(gui->PersoServerSendEchoButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerSendEchoButton_slot);

  connect(gui->AutoProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_AutoProgramDeviceButton_slot);
  connect(gui->ManualProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_ManualProgramDeviceButton_slot);
  connect(gui->ReadDeviceFirmwareButton, &QPushButton::clicked, this,
          &MainWindow::on_ReadDeviceFirmwareButton_slot);
  connect(gui->EraseDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_EraseDeviceButton_slot);

  connect(gui->ReadDeviceUserDataButton, &QPushButton::clicked, this,
          &MainWindow::on_ReadDeviceUserDataButton_slot);
  connect(gui->ProgramDeviceUserDataButton, &QPushButton::clicked, this,
          &MainWindow::on_ProgramDeviceUserDataButton_slot);

  connect(gui->UnlockDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_UnlockDeviceButton_slot);
  connect(gui->LockDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_LockDeviceButton_slot);

  connect(gui->ApplySettingsPushButton, &QPushButton::clicked, this,
          &MainWindow::on_ApplySettingsPushButton_slot);

  // Подключение системы логгирования
  connect(Logger, &LogSystem::requestDisplayLog,
          dynamic_cast<MasterGUI*>(CurrentGUI), &MasterGUI::displayLogData);
  connect(Logger, &LogSystem::requestClearDisplayLog,
          dynamic_cast<MasterGUI*>(CurrentGUI),
          &MasterGUI::clearLogDataDisplay);
}

void MainWindow::createProductionInterface() {
  // Создаем интерфейс
  delete CurrentGUI;
  CurrentGUI = new ProductionGUI(this);
  CurrentGUI->create();
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();

  // Включаем систему логгирования
  Logger->setEnable(false);
}

void MainWindow::connectProductionInterface() {
  ProductionGUI *gui = dynamic_cast<ProductionGUI *>(CurrentGUI);

  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_AutoProgramDeviceButton_slot);
}

void MainWindow::createTopMenuActions() {
  if (CurrentGUI->type() == GUI::Production) {
    MasterInterfaceRequestAct = new QAction("Мастер интерфейс", this);
    MasterInterfaceRequestAct->setStatusTip("Открыть мастер интерфейс");
    connect(MasterInterfaceRequestAct, &QAction::triggered, this,
            &MainWindow::on_MasterInterfaceRequestAct_slot);
  } else {
    ProductionInterfaceRequestAct =
        new QAction("Производственный интерфейс", this);
    ProductionInterfaceRequestAct->setStatusTip(
        "Открыть производственный интерфейс");
    connect(ProductionInterfaceRequestAct, &QAction::triggered, this,
            &MainWindow::on_ProductionInterfaceRequestAct_slot);
  }

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void MainWindow::createTopMenu() {
  // Удаляем предыдущее топ меню
  menuBar()->clear();

  // Cоздаем обработчики пользовательских действий
  createTopMenuActions();

  // Создаем меню
  ServiceMenu = menuBar()->addMenu("Сервис");
  if (CurrentGUI->type() == GUI::Production)
    ServiceMenu->addAction(MasterInterfaceRequestAct);
  else
    ServiceMenu->addAction(ProductionInterfaceRequestAct);

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}
