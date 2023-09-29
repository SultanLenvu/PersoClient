#include "mainwindow_kernel.h"

MainWindow::MainWindow() {
  setObjectName("MainWindow");
  CurrentGUI = nullptr;

  // Загружаем настройки приложения
  loadSettings();

  // Система для взаимодействия с пользователем
  Interactor = new UserInteractionSystem(this, this);

  // Создаем логгер
  Logger = new LogSystem(this);

  // Создаем модель для представления данных транспондера
  SeedModel = new TransponderSeedModel(this);

  // Менеджер для взаимодействия с программатором
  createManager();

  // Создаем графический интерфейс для авторизации
  createMasterInterface();
}

MainWindow::~MainWindow() {}

void MainWindow::on_AuthorizePushButton_slot() {
  QMap<QString, QString> data;
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);
  data.insert("Login", gui->LoginLineEdit->text());
  data.insert("Password", gui->PasswordLineEdit->text());

  Manager->performServerAuthorization(&data);

  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем графический интерфейс
  createProductionInterface();
}

void MainWindow::on_ProgramDeviceButton_slot() {
  Logger->clear();

  Manager->performLocalFirmwareLoading(
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

  Manager->performDataLoading(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)"));
}

void MainWindow::on_ReadDeviceUserDataButton_slot() {
  Logger->clear();

  Manager->performDataReading();
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
    Interactor->generateErrorMessage(
        "Введены некорректные данные для настроек. ");
    return;
  }

  // Считывание пользовательского ввода
  Settings->setValue("Personalization/ServerIpAddress",
                     gui->PersoServerIpAddressLineEdit->text());
  Settings->setValue("Personalization/ServerPort",
                     gui->PersoServerPortLineEdit->text().toInt());
  Settings->setValue("JLinkExeProgrammer/ExeFile/Path",
                     gui->ProgrammerExeFilePathLineEdit->text());
  Settings->setValue("StickerPrinter/DLL/Path",
                     gui->PrinterDllPathLineEdit->text());

  // Применение новых настроек
  Manager->applySettings();

  Interactor->generateMessage("Новые настройки успешно применены. ");
}

void MainWindow::on_PersoServerConnectPushButton_slot() {
  Logger->clear();

  Manager->performServerConnecting();
}

void MainWindow::on_PersoServerDisconnectButton_slot() {
  Logger->clear();

  Manager->performServerDisconnecting();
}

void MainWindow::on_PersoServerEchoRequestButton_slot() {
  Logger->clear();

  Manager->performServerEcho();
}

void MainWindow::on_MasterAuthorizePushButton_slot() {
  QMap<QString, QString> data;
  Interactor->getAuthorizationData(&data);

  if (data.isEmpty()) {
    return;
  }

  Manager->performServerAuthorization(&data);
}

void MainWindow::on_LoadTransponderFirmwareButton_slot() {
  Logger->clear();

  Manager->performTransponderFirmwareLoading(SeedModel);
}

void MainWindow::on_ReloadTransponderFirmwareButton_slot() {
  Logger->clear();

  QString pan = getStickerPan();

  if (pan.isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных");
    return;
  }

  Manager->performTransponderFirmwareReloading(SeedModel, pan);
}

void MainWindow::on_MasterInterfaceRequest_slot() {
  QString pass;
  Interactor->getMasterPassword(pass);

  if (pass != QString(MASTER_ACCESS_PASSWORD)) {
    Interactor->generateErrorMessage("Неверный пароль");
    return;
  }

  // Создаем мастер интерфейс
  createMasterInterface();
}

void MainWindow::on_ProductionInterfaceRequest_slot() {
  createProductionInterface();
}

void MainWindow::proxyLogging(const QString& log) {
  if (sender()->objectName() == QString("ClientManager")) {
    Logger->generate(QString("Manager - ") + log);
  } else {
    Logger->generate(QString("Unknown - ") + log);
  }
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

  QFileInfo info(gui->ProgrammerExeFilePathLineEdit->text());
  if ((!info.exists()) || (!info.isExecutable())) {
    return false;
  }

  info.setFile(gui->PrinterDllPathLineEdit->text());
  if ((!info.exists()) || (info.suffix() != "dll")) {
    return false;
  }

  return true;
}

QString MainWindow::getStickerPan(void) {
  QStringList stickerData;

  Interactor->getTransponderStickerData(&stickerData);

  if (stickerData.size() > 2) {
    return QString();
  }

  if (stickerData.at(0).size() == PAN_CHAR_LENGTH) {
    return stickerData.at(0);
  }

  if (stickerData.at(1).size() == PAN_CHAR_LENGTH) {
    return stickerData.at(1);
  }

  return QString();
}

void MainWindow::createAuthorizationInterface() {
  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.15, DesktopGeometry.height() * 0.1);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  delete CurrentGUI;
  CurrentGUI = new AuthorizationGUI(this);
  CurrentGUI->create();
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectAuthorizationInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindow::connectAuthorizationInterface() {
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &MainWindow::on_AuthorizePushButton_slot);
}

void MainWindow::createMasterInterface() {
  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  delete CurrentGUI;
  CurrentGUI = new MasterGUI(this);
  CurrentGUI->create();
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectMasterInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindow::connectMasterInterface() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);

  // Сервер
  connect(gui->PersoServerConnectPushButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerConnectPushButton_slot);
  connect(gui->PersoServerDisconnectButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerDisconnectButton_slot);
  connect(gui->PersoServerEchoRequestButton, &QPushButton::clicked, this,
          &MainWindow::on_PersoServerEchoRequestButton_slot);
  connect(gui->MasterAuthorizePushButton, &QPushButton::clicked, this,
          &MainWindow::on_MasterAuthorizePushButton_slot);
  connect(gui->LoadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindow::on_LoadTransponderFirmwareButton_slot);
  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindow::on_ReloadTransponderFirmwareButton_slot);

  // Программатор
  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_ProgramDeviceButton_slot);
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

  // Система логгирования
  connect(Logger, &LogSystem::requestDisplayLog,
          dynamic_cast<MasterGUI*>(CurrentGUI), &MasterGUI::displayLogData);
  connect(Logger, &LogSystem::requestClearDisplayLog,
          dynamic_cast<MasterGUI*>(CurrentGUI),
          &MasterGUI::clearLogDataDisplay);
}

void MainWindow::createProductionInterface() {
  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  delete CurrentGUI;
  CurrentGUI = new ProductionGUI(this);
  CurrentGUI->create();
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindow::connectProductionInterface() {
  ProductionGUI *gui = dynamic_cast<ProductionGUI *>(CurrentGUI);

  connect(gui->LoadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindow::on_LoadTransponderFirmwareButton_slot);
  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindow::on_ReloadTransponderFirmwareButton_slot);
}

void MainWindow::createTopMenuActions() {
  if ((CurrentGUI->type() == GUI::Production) ||
      (CurrentGUI->type() == GUI::Authorization)) {
    MasterInterfaceRequestAct = new QAction("Мастер интерфейс", this);
    MasterInterfaceRequestAct->setStatusTip("Открыть мастер интерфейс");
    connect(MasterInterfaceRequestAct, &QAction::triggered, this,
            &MainWindow::on_MasterInterfaceRequest_slot);
  } else {
    ProductionInterfaceRequestAct =
        new QAction("Производственный интерфейс", this);
    ProductionInterfaceRequestAct->setStatusTip(
        "Открыть производственный интерфейс");
    connect(ProductionInterfaceRequestAct, &QAction::triggered, this,
            &MainWindow::on_ProductionInterfaceRequest_slot);
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
  if ((CurrentGUI->type() == GUI::Production) ||
      (CurrentGUI->type() == GUI::Authorization)) {
    ServiceMenu->addAction(MasterInterfaceRequestAct);
  } else {
    ServiceMenu->addAction(ProductionInterfaceRequestAct);
  }

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void MainWindow::createManager() {
  Manager = new ClientManager(this);
  connect(Manager, &ClientManager::logging, this, &MainWindow::proxyLogging);
  connect(Manager, &ClientManager::notifyUser, Interactor,
          &UserInteractionSystem::generateMessage);
  connect(Manager, &ClientManager::notifyUserAboutError, Interactor,
          &UserInteractionSystem::generateErrorMessage);
  connect(Manager, &ClientManager::operationPerfomingStarted, Interactor,
          &UserInteractionSystem::generateProgressDialog);
  connect(Manager, &ClientManager::operationStepPerfomed, Interactor,
          &UserInteractionSystem::performeProgressDialogStep);
  connect(Manager, &ClientManager::operationPerformingEnded, Interactor,
          &UserInteractionSystem::completeProgressDialog);
  connect(Manager, &ClientManager::createProductionInterface_signal, this,
          &MainWindow::on_ProductionInterfaceRequest_slot);
}
