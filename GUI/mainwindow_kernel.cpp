#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "GUI/authorization_gui.h"
#include "General/definitions.h"
#include "Log/widget_log_backend.h"
#include "definitions.h"
#include "mainwindow_kernel.h"
#include "master_gui.h"
#include "production_gui.h"
#include "testing_gui.h"

MainWindowKernel::MainWindowKernel(QWidget* parent) : QMainWindow(parent) {
  setObjectName("MainWindowKernel");
  CurrentGUI = nullptr;
  DesktopGeometry = QApplication::screens().first()->size();

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

  // Создаем модели
  createModels();

  // Создаем графический интерфейс для авторизации
  createAuthorizationInterface();

  // Регистрируем типы
  registerMetaTypes();
}

MainWindowKernel::~MainWindowKernel() {
  ManagerThread->exit();
  ManagerThread->wait();

  LoggerThread->exit();
  LoggerThread->wait();
}

void MainWindowKernel::on_AuthorizePushButton_slot() {
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);

  if (gui->ModeChoice->currentText() == "Производство") {
    QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);
    AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);
    data->insert("login", gui->LoginLineEdit->text());
    data->insert("password", gui->PasswordLineEdit->text());

    emit performServerAuthorization_signal(data);
  }

  if (gui->ModeChoice->currentText() == "Тестирование") {
    createTestingInterface();
  }
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

  emit performLastTransponderStickerPrinting_signal();
}

void MainWindowKernel::on_PrintCustomTransponderStickerButton_slot() {
  QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);

  emit loggerClear_signal();

  if (!Interactor->getCustomTransponderStickerData(data.get())) {
    return;
  }

  if (data->isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  emit performCustomTransponderStickerPrinting_signal(data);
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

  settings.setValue("perso_client/server_ip",
                    gui->PersoServerIpAddressLineEdit->text());
  settings.setValue("perso_client/server_port",
                    gui->PersoServerPortLineEdit->text().toInt());

  settings.setValue("jlink_exe_programmer/exe_file_path",
                    gui->ProgrammerExeFilePathLineEdit->text());
  settings.setValue("jlink_exe_programmer/speed",
                    gui->ProgrammerSpeedLineEdit->text());

  settings.setValue("te310_printer/library_path",
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
  QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);
  Interactor->getAuthorizationData(data.get());

  if (data->isEmpty()) {
    return;
  }

  emit performServerAuthorization_signal(data);
}

void MainWindowKernel::on_LoadTransponderFirmwareButton_slot() {
  emit loggerClear_signal();

  emit performTransponderFirmwareLoading_signal();
}

void MainWindowKernel::on_ReloadTransponderFirmwareButton_slot() {
  emit loggerClear_signal();

  QHash<QString, QString> stickerData;
  if (!Interactor->getTransponderStickerData(&stickerData)) {
    return;
  }

  if (stickerData.isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  emit performTransponderFirmwareReloading_signal(stickerData.value("pan"));
}

void MainWindowKernel::on_RollbackProductionLinePushButton_slot() {
  emit loggerClear_signal();

  emit rollbackProductionLine_signal();
}

void MainWindowKernel::on_PrintBoxStickerButton_slot() {
  QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);

  emit loggerClear_signal();

  if (!Interactor->getTransponderStickerData(data.get())) {
    return;
  }

  if (data->isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  emit performBoxStickerPrinting_signal(data);
}

void MainWindowKernel::on_PrintPalletStickerButton_slot() {
  QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);

  emit loggerClear_signal();

  if (!Interactor->getTransponderStickerData(data.get())) {
    return;
  }

  if (data->isEmpty()) {
    Interactor->generateErrorMessage("Некорректный ввод данных. ");
    return;
  }

  emit performPalletStickerPrinting_signal(data);
}

void MainWindowKernel::on_MasterInterfaceRequest_slot() {
  QString pass;
  if (!Interactor->getMasterPassword(pass)) {
    return;
  }

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

void MainWindowKernel::on_AuthorizationInterfaceRequest_slot() {
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

  return true;
}

void MainWindowKernel::createAuthorizationInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.1);
  setLayoutDirection(Qt::LeftToRight);
  setMinimumWidth(DesktopGeometry.width() * 0.2);

  // Создаем интерфейс
  CurrentGUI = new AuthorizationGUI(this);
  setCentralWidget(CurrentGUI);
  CurrentGUI->showMaximized();

  adjustSize();
  setFixedSize(size());

  // Подключаем интерфейс
  connectAuthorizationInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindowKernel::connectAuthorizationInterface() {
  AuthorizationGUI* gui = dynamic_cast<AuthorizationGUI*>(CurrentGUI);

  connect(gui->AuthorizePushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_AuthorizePushButton_slot);
  connect(gui, &AbstractGUI::visibilityChanged, this,
          &MainWindowKernel::on_VisibilityChanged_slot);
}

void MainWindowKernel::createMasterInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  CurrentGUI = new MasterGUI(this);
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
  connect(gui->RollbackProductionLinePushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_RollbackProductionLinePushButton_slot);

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
  gui->TransponderDataView->setModel(TransponderDataModel);
}

void MainWindowKernel::createProductionInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGUI = new ProductionGUI(this);
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindowKernel::connectProductionInterface() {
  ProductionGUI* gui = dynamic_cast<ProductionGUI*>(CurrentGUI);

  connect(gui->LoadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_LoadTransponderFirmwareButton_slot);
  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReloadTransponderFirmwareButton_slot);
  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintLastTransponderStickerButton_slot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintCustomTransponderStickerButton_slot);
  connect(gui->RollbackProductionLinePushButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_RollbackProductionLinePushButton_slot);
  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintBoxStickerButton_slot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel);
}

void MainWindowKernel::createTestingInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGUI = new TestingGUI(this);
  setCentralWidget(CurrentGUI);

  // Подключаем интерфейс
  connectTestingInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindowKernel::connectTestingInterface() {
  TestingGUI* gui = dynamic_cast<TestingGUI*>(CurrentGUI);

  connect(gui->ReloadTransponderFirmwareButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_ReloadTransponderFirmwareButton_slot);
  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintLastTransponderStickerButton_slot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintCustomTransponderStickerButton_slot);
  connect(gui->PrintBoxStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintBoxStickerButton_slot);
  connect(gui->PrintPalletStickerButton, &QPushButton::clicked, this,
          &MainWindowKernel::on_PrintPalletStickerButton_slot);

  // Связывание моделей и представлений
  gui->TransponderDataView->setModel(TransponderDataModel);
}

void MainWindowKernel::createTopMenuActions() {
  OpenMasterInterfaceAct = new QAction("Мастер доступ", this);
  OpenMasterInterfaceAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterInterfaceAct, &QAction::triggered, this,
          &MainWindowKernel::on_MasterInterfaceRequest_slot);

  OpenAuthorizationInterfaceAct = new QAction("Авторизация", this);
  OpenAuthorizationInterfaceAct->setStatusTip("Открыть интерфейс авторизации");
  connect(OpenAuthorizationInterfaceAct, &QAction::triggered, this,
          &MainWindowKernel::on_AuthorizationInterfaceRequest_slot);

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void MainWindowKernel::createTopMenu() {
  // Удаляем предыдущее топ меню
  menuBar()->clear();

  // Создаем меню
  ServiceMenu = menuBar()->addMenu("Сервис");
  switch (CurrentGUI->type()) {
    case AbstractGUI::Testing:
    case AbstractGUI::Production:
      ServiceMenu->addAction(OpenMasterInterfaceAct);
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case AbstractGUI::Master:
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case AbstractGUI::Authorization:
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
  connect(Manager, &ClientManager::operationPerformingStarted, Interactor,
          &InteractionSystem::startOperationProgressDialog);
  connect(Manager, &ClientManager::operationPerformingFinished, Interactor,
          &InteractionSystem::finishOperationProgressDialog);
  connect(Manager, &ClientManager::requestProductionInterface_signal, this,
          &MainWindowKernel::on_RequestProductionInterface_slot);
  connect(Manager, &ClientManager::displayTransponderData_signal, this,
          &MainWindowKernel::displayTransponderData_slot);

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
  connect(this, &MainWindowKernel::rollbackProductionLine_signal, Manager,
          &ClientManager::rollbackProductionLine);
  connect(this, &MainWindowKernel::performBoxStickerPrinting_signal, Manager,
          &ClientManager::performBoxStickerPrinting);
  connect(this, &MainWindowKernel::performPalletStickerPrinting_signal, Manager,
          &ClientManager::performPalletStickerPrinting);

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

  connect(this, &MainWindowKernel::performLastTransponderStickerPrinting_signal,
          Manager, &ClientManager::performLastTransponderStickerPrinting);
  connect(this,
          &MainWindowKernel::performCustomTransponderStickerPrinting_signal,
          Manager, &ClientManager::performCustomTransponderStickerPrinting);
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
          &ClientManager::on_InstanceThreadStarted_slot);

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

void MainWindowKernel::createModels() {
  TransponderDataModel = new HashModel(this);
}

void MainWindowKernel::registerMetaTypes() {
  qRegisterMetaType<QSharedPointer<QHash<QString, QString>>>(
      "QSharedPointer<QHash<QString, QString> >");
}

void MainWindowKernel::on_RequestProductionInterface_slot() {
  createProductionInterface();
}

void MainWindowKernel::on_VisibilityChanged_slot() {
  adjustSize();
  setFixedSize(size());
}

void MainWindowKernel::displayTransponderData_slot(
    QSharedPointer<QHash<QString, QString>> data) {
  TransponderDataModel->buildTransponderData(data.get());
  CurrentGUI->update();
}
