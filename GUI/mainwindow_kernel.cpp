#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "General/definitions.h"
#include "mainwindow_kernel.h"
#include "Log/widget_log_backend.h"

MainWindowKernel::MainWindowKernel(QWidget* parent) : QMainWindow(parent) {
  setObjectName("MainWindowKernel");
  CurrentGUI = nullptr;

  // Загружаем настройки приложения
  loadSettings();

  // Создаем действия для верхнего меню
  createTopMenuActions();

  // Создаем логгер
  createLoggerInstance();

  // Система для взаимодействия с пользователем
  createInteractorInstance();

  // Менеджер для взаимодействия с программатором
  createManagerInstance();

  // Создаем модель для представления данных транспондера
  TransponderInfo = new MapModel(this);

  // Создаем графический интерфейс для авторизации
  //  createAuthorizationInterface();

  // Debug
  createMasterInterface();

  // Регистрируем типы
  qRegisterMetaType<QSharedPointer<QMap<QString, QString>>>(
      "QSharedPointer<QMap<QString, QString> >");
}

MainWindowKernel::~MainWindowKernel() {
  ManagerThread->exit();
  ManagerThread->wait();

  LoggerThread->exit();
  LoggerThread->wait();
}

void MainWindowKernel::on_AuthorizePushButton_slot() {
  QSharedPointer<QMap<QString, QString>> data(new QMap<QString, QString>);
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);
  data->insert("login", gui->LoginLineEdit->text());
  data->insert("password", gui->PasswordLineEdit->text());

  emit performServerAuthorization_signal(data);
}

void MainWindowKernel::on_ProgramDeviceButton_slot() {
  emit loggerClear_signal();

  emit performLocalFirmwareLoading_signal(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)"));
}

void MainWindowKernel::on_ReadDeviceFirmwareButton_slot() {
  emit loggerClear_signal();

  emit performFirmwareReading_signal();
}

void MainWindowKernel::on_EraseDeviceButton_slot() {
  emit loggerClear_signal();

  emit performFirmwareErasing_signal();
}

void MainWindowKernel::on_ProgramDeviceUserDataButton_slot() {
  emit loggerClear_signal();

  emit performDataLoading_signal(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)"));
}

void MainWindowKernel::on_ReadDeviceUserDataButton_slot() {
  emit loggerClear_signal();

  emit performDataReading_signal();
}

void MainWindowKernel::on_UnlockDeviceButton_slot() {
  emit loggerClear_signal();

  emit performDeviceUnlock_signal();
}

void MainWindowKernel::on_LockDeviceButton_slot() {
  emit loggerClear_signal();

  emit performDeviceLock_signal();
}

void MainWindowKernel::on_PrintLastTransponderStickerButton_slot() {
  emit loggerClear_signal();

  emit performPrintingLastTransponderSticker_signal();
}

void MainWindowKernel::on_PrintCustomTransponderStickerButton_slot() {
  emit loggerClear_signal();

  QSharedPointer<QMap<QString, QString>> data(new QMap<QString, QString>);
  bool ok = false;
  Interactor->getCustomTransponderStickerData(data.get(), ok);

  if (!ok) {
    return;
  }

  if (data->isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  emit performPrintingCustomTransponderSticker_signal(data);
}

void MainWindowKernel::on_ExecuteStickerPrinterCommandScriptButton_slot() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);
  emit loggerClear_signal();

  QSharedPointer<QStringList> commandScript(new QStringList(
      gui->StickerPrinterCommandSriptTextEdit->toPlainText().split("\n\r")));
  emit performStickerPrinterCommandScript_signal(commandScript);
}

void MainWindowKernel::on_ApplySettingsPushButton_slot() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);
  QSettings settings;

  emit loggerClear_signal();

  // Проверка пользовательского ввода
  if (!checkNewSettings()) {
    Interactor->generateErrorMessage(
        "Введены некорректные данные для настроек. ");
    return;
  }

  settings.setValue("log_system/global_enable",
                    gui->LogSystemGlobalEnableCheckBox->isChecked());
  settings.setValue("log_system/extended_enable",
                    gui->LogSystemExtendedEnableCheckBox->isChecked());
  settings.setValue("log_system/save_path",
                    gui->LogSystemSavePathLineEdit->text());

  settings.setValue("perso_client/server_ip",
                    gui->PersoServerIpAddressLineEdit->text());
  settings.setValue("perso_client/server_port",
                    gui->PersoServerPortLineEdit->text().toInt());

  settings.setValue("jlink_exe_programmer/exe_file_path",
                    gui->ProgrammerExeFilePathLineEdit->text());
  settings.setValue("jlink_exe_programmer/speed",
                    gui->ProgrammerSpeedLineEdit->text());

  settings.setValue("sticker_printer/library_path",
                    gui->StickerPrinterLibPathLineEdit->text());

  // Применение новых настроек
  emit applySettings_signal();
  Interactor->generateMessage("Новые настройки успешно применены. ");
}

void MainWindowKernel::on_PersoServerConnectPushButton_slot() {
  emit loggerClear_signal();

  emit performServerConnecting_signal();
}

void MainWindowKernel::on_PersoServerDisconnectButton_slot() {
  emit loggerClear_signal();

  emit performServerDisconnecting_signal();
}

void MainWindowKernel::on_PersoServerEchoRequestButton_slot() {
  emit loggerClear_signal();

  emit performServerEcho_signal();
}

void MainWindowKernel::on_MasterAuthorizePushButton_slot() {
  QSharedPointer<QMap<QString, QString>> data(new QMap<QString, QString>);
  Interactor->getAuthorizationData(data.get());

  if (data->isEmpty()) {
    return;
  }

  emit performServerAuthorization_signal(data);
}

void MainWindowKernel::on_LoadTransponderFirmwareButton_slot() {
  emit loggerClear_signal();

  emit performTransponderFirmwareLoading_signal(TransponderInfo);

  CurrentGUI->update();
}

void MainWindowKernel::on_ReloadTransponderFirmwareButton_slot() {
  emit loggerClear_signal();

  QStringList stickerData;
  bool ok;
  Interactor->getTransponderStickerData(&stickerData, ok);
  if (!ok) {
    return;
  }
  if (stickerData.isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  QString pan = getStickerPan(stickerData);
  if (pan.isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных");
    return;
  }

  emit performTransponderFirmwareReloading_signal(TransponderInfo, pan);

  CurrentGUI->update();
}

void MainWindowKernel::on_MasterInterfaceRequest_slot() {
  QString pass;
  Interactor->getMasterPassword(pass);

  if (pass != QString(MASTER_ACCESS_PASSWORD)) {
    Interactor->generateErrorMessage("Неверный пароль");
    return;
  }

  // Создаем мастер интерфейс
  createMasterInterface();
}

void MainWindowKernel::on_ProductionInterfaceRequest_slot() {
  createProductionInterface();
}

void MainWindowKernel::on_OpenAuthorizationInterfaceAct_slot() {
  createAuthorizationInterface();
}

void MainWindowKernel::loadSettings() {
  QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(PROGRAM_NAME);

  QSettings::setDefaultFormat(QSettings::IniFormat);
  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                     QCoreApplication::applicationDirPath());
}

bool MainWindowKernel::checkNewSettings() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);

  QHostAddress IP = QHostAddress(gui->PersoServerIpAddressLineEdit->text());
  uint32_t speed = gui->ProgrammerSpeedLineEdit->text().toUInt();

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

  if (speed == 0) {
    return false;
  }

  info.setFile(gui->StickerPrinterLibPathLineEdit->text());
  if ((!info.exists()) || (info.suffix() != "dll")) {
    return false;
  }

  info.setFile(gui->LogSystemSavePathLineEdit->text());
  if (!info.isDir()) {
    return false;
  }

  return true;
}

QString MainWindowKernel::getStickerPan(QStringList& stickerData) {
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

void MainWindowKernel::createAuthorizationInterface() {
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

void MainWindowKernel::connectAuthorizationInterface() {
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_AuthorizePushButton_slot);
}

void MainWindowKernel::createMasterInterface() {
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

void MainWindowKernel::connectMasterInterface() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGUI);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::displayLog_signal, gui,
          &MasterGUI::displayLogData);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::clearLogDisplay_signal, gui,
          &MasterGUI::clearLogDataDisplay);

  // Сервер
  connect(gui->PersoServerConnectPushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PersoServerConnectPushButton_slot);
  connect(gui->PersoServerDisconnectButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PersoServerDisconnectButton_slot);
  connect(gui->PersoServerEchoRequestButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PersoServerEchoRequestButton_slot);
  connect(gui->MasterAuthorizePushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_MasterAuthorizePushButton_slot);
  connect(gui->LoadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_LoadTransponderFirmwareButton_slot);
  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReloadTransponderFirmwareButton_slot);

  // Программатор
  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ProgramDeviceButton_slot);
  connect(gui->ReadDeviceFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReadDeviceFirmwareButton_slot);
  connect(gui->EraseDeviceButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_EraseDeviceButton_slot);
  connect(gui->ReadDeviceUserDataButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReadDeviceUserDataButton_slot);
  connect(gui->ProgramDeviceUserDataButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ProgramDeviceUserDataButton_slot);
  connect(gui->UnlockDeviceButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_UnlockDeviceButton_slot);
  connect(gui->LockDeviceButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_LockDeviceButton_slot);

  // Стикер принтер
  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintLastTransponderStickerButton_slot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintCustomTransponderStickerButton_slot);
  connect(gui->ExecuteStickerPrinterCommandScriptButton, &QPushButton::clicked,
          this,
          &MainWindowKernel::on_ExecuteStickerPrinterCommandScriptButton_slot);

  // Настройки
  connect(gui->ApplySettingsPushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ApplySettingsPushButton_slot);

  // Связывание моделей и представлений
  gui->TransponderInfoView->setModel(TransponderInfo);
}

void MainWindowKernel::createProductionInterface() {
  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  delete CurrentGUI;
  CurrentGUI = new ProductionGUI(this);
  CurrentGUI->create();
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindowKernel::connectProductionInterface() {
  ProductionGUI *gui = dynamic_cast<ProductionGUI *>(CurrentGUI);

  connect(gui->LoadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_LoadTransponderFirmwareButton_slot);
  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReloadTransponderFirmwareButton_slot);
  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintLastTransponderStickerButton_slot);

  // Связывание моделей и представлений
  gui->TransponderInfoView->setModel(TransponderInfo);
}

void MainWindowKernel::createTopMenuActions() {
  OpenMasterInterfaceAct = new QAction("Мастер доступ", this);
  OpenMasterInterfaceAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterInterfaceAct, &QAction::triggered, this,
          &MainWindowKernel::on_MasterInterfaceRequest_slot);
  OpenAuthorizationInterfaceAct =
      new QAction("Выйти из производственной линии", this);

  OpenAuthorizationInterfaceAct->setStatusTip("Авторизация");
  OpenAuthorizationInterfaceAct->setStatusTip("Открыть интерфейс авторизации");
  connect(OpenAuthorizationInterfaceAct, &QAction::triggered, this,
          &MainWindowKernel::on_OpenAuthorizationInterfaceAct_slot);

  OpenProductionInterfaceAct = new QAction("Производственный доступ", this);
  OpenProductionInterfaceAct->setStatusTip(
      "Открыть производственный интерфейс");
  connect(OpenProductionInterfaceAct, &QAction::triggered, this,
          &MainWindowKernel::on_ProductionInterfaceRequest_slot);

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void MainWindowKernel::createTopMenu() {
  // Удаляем предыдущее топ меню
  menuBar()->clear();

  // Создаем меню
  ServiceMenu = menuBar()->addMenu("Сервис");
  switch (CurrentGUI->type()) {
    case GUI::Production:
      ServiceMenu->addAction(OpenMasterInterfaceAct);
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case GUI::Master:
      ServiceMenu->addAction(OpenProductionInterfaceAct);
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case GUI::Authorization:
      ServiceMenu->addAction(OpenMasterInterfaceAct);
      break;
  }

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void MainWindowKernel::createManagerInstance() {
  Manager = new ClientManager(nullptr);
  connect(Manager, &ClientManager::logging, LogSystem::instance(),
          &LogSystem::generate);
  connect(Manager, &ClientManager::notifyUser, Interactor,
          &InteractionSystem::generateMessage);
  connect(Manager, &ClientManager::notifyUserAboutError, Interactor,
          &InteractionSystem::generateErrorMessage);
  connect(Manager, &ClientManager::operationPerfomingStarted, Interactor,
          &InteractionSystem::startOperationProgressDialog);
  connect(Manager, &ClientManager::operationPerformingFinished, Interactor,
          &InteractionSystem::finishOperationProgressDialog);
  connect(Manager, &ClientManager::requestProductionInterface_signal, this,
          &MainWindowKernel::on_RequestProductionInterface_slot);

  // Подключаем функционал
  connect(this, &MainWindowKernel::performServerConnecting_signal, Manager,
          &ClientManager::performServerConnecting);
  connect(this, &MainWindowKernel::performServerDisconnecting_signal, Manager,
          &ClientManager::performServerDisconnecting);
  connect(this, &MainWindowKernel::performServerEcho_signal, Manager,
          &ClientManager::performServerEcho);
  connect(this, &MainWindowKernel::performServerAuthorization_signal, Manager,
          &ClientManager::performServerAuthorization);
  connect(this, &MainWindowKernel::performTransponderFirmwareLoading_signal,
          Manager, &ClientManager::performTransponderFirmwareLoading);
  connect(this, &MainWindowKernel::performTransponderFirmwareReloading_signal,
          Manager, &ClientManager::performTransponderFirmwareReloading);
  connect(this, &MainWindowKernel::performLocalFirmwareLoading_signal, Manager,
          &ClientManager::performLocalFirmwareLoading);
  connect(this, &MainWindowKernel::performFirmwareReading_signal, Manager,
          &ClientManager::performFirmwareReading);
  connect(this, &MainWindowKernel::performFirmwareErasing_signal, Manager,
          &ClientManager::performFirmwareErasing);
  connect(this, &MainWindowKernel::performDataReading_signal, Manager,
          &ClientManager::performDataReading);
  connect(this, &MainWindowKernel::performDataLoading_signal, Manager,
          &ClientManager::performDataLoading);
  connect(this, &MainWindowKernel::performDeviceUnlock_signal, Manager,
          &ClientManager::performDeviceUnlock);
  connect(this, &MainWindowKernel::performDeviceLock_signal, Manager,
          &ClientManager::performDeviceLock);
  connect(this, &MainWindowKernel::performPrintingLastTransponderSticker_signal,
          Manager, &ClientManager::performPrintingLastTransponderSticker);
  connect(this,
          &MainWindowKernel::performPrintingCustomTransponderSticker_signal,
          Manager, &ClientManager::performPrintingCustomTransponderSticker);
  connect(this, &MainWindowKernel::performStickerPrinterCommandScript_signal,
          Manager, &ClientManager::performStickerPrinterCommandScript);
  connect(this, &MainWindowKernel::applySettings_signal, Manager,
          &ClientManager::applySettings);

  // Создаем отдельный поток
  ManagerThread = new QThread(this);
  connect(ManagerThread, &QThread::finished, ManagerThread,
          &QThread::deleteLater);
  connect(ManagerThread, &QThread::finished, Manager,
          &ClientManager::deleteLater);
  connect(ManagerThread, &QThread::started, Manager,
          &ClientManager::on_InsctanceThreadStarted_slot);

  Manager->moveToThread(ManagerThread);
  ManagerThread->start();
}

void MainWindowKernel::createLoggerInstance() {
  Logger = LogSystem::instance();
  connect(this, &MainWindowKernel::applySettings_signal, Logger,
          &LogSystem::applySettings);
  connect(this, &MainWindowKernel::loggerClear_signal, Logger,
          &LogSystem::clear);

  LoggerThread = new QThread(this);
  connect(LoggerThread, &QThread::finished, LoggerThread,
          &QThread::deleteLater);

  LogSystem::instance()->moveToThread(LoggerThread);
  LoggerThread->start();
}

void MainWindowKernel::createInteractorInstance() {
  Interactor = InteractionSystem::instance();
  connect(Interactor, &InteractionSystem::logging, Logger,
          &LogSystem::generate);
  connect(this, &MainWindowKernel::applySettings_signal, Interactor,
          &InteractionSystem::applySettings);
}

void MainWindowKernel::on_RequestProductionInterface_slot() {
  createProductionInterface();
}
