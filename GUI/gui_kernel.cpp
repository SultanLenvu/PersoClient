#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "GUI/authorization_gui.h"
#include "General/definitions.h"
#include "Log/widget_log_backend.h"
#include "definitions.h"
#include "gui_kernel.h"
#include "master_gui.h"
#include "network_manager.h"
#include "production_gui.h"
#include "programmer_manager.h"
#include "sticker_printer_manager.h"
#include "testing_gui.h"

GuiKernel::GuiKernel(QWidget* parent) : QMainWindow(parent) {
  setObjectName("GuiKernel");
  CurrentGui = nullptr;
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
  createManagersInstance();

  // Создаем модели
  createModels();

  // Создаем графический интерфейс для авторизации
  createAuthorizationInterface();

  // Регистрируем типы
  registerMetaTypes();
}

GuiKernel::~GuiKernel() {
  ManagersThread->exit();
  ManagersThread->wait();

  LoggerThread->exit();
  LoggerThread->wait();
}

void GuiKernel::applySettingsPushButton_slot() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui);
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

void GuiKernel::on_AuthorizePushButton_slot() {
  QSharedPointer<QHash<QString, QString>> data(new QHash<QString, QString>);
  Interactor->getAuthorizationData(data.get());

  if (data->isEmpty()) {
    return;
  }

  emit performServerAuthorization_signal(data);
}

void GuiKernel::displayMasterInterface_slot() {
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

void GuiKernel::displayProductionInterface_slot() {
  createProductionInterface();
}

void GuiKernel::displayAuthorizationInterface_slot() {
  createAuthorizationInterface();
}

void GuiKernel::loadSettings() {
  QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(PROGRAM_NAME);

  QSettings::setDefaultFormat(QSettings::IniFormat);
  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                     QCoreApplication::applicationDirPath());
}

bool GuiKernel::checkNewSettings() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui);

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

void GuiKernel::createAuthorizationInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.1);
  setLayoutDirection(Qt::LeftToRight);
  setMinimumWidth(DesktopGeometry.width() * 0.2);

  // Создаем интерфейс
  CurrentGui = std::shared_ptr<AbstractGui>(new AuthorizationGui(this));
  setCentralWidget(CurrentGui.get());
  CurrentGui->showMaximized();

  adjustSize();
  setFixedSize(size());

  // Подключаем интерфейс
  connectAuthorizationInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectAuthorizationInterface() {
  AuthorizationGui* gui = dynamic_cast<AuthorizationGui*>(CurrentGui.get());

  connect(gui, &AbstractGui::visibilityChanged, this,
          &GuiKernel::currentGuiVisibilityChanged_slot);
}

void GuiKernel::createMasterInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  CurrentGui = new MasterGUI(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectMasterInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectMasterInterface() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::displayLog_signal, gui,
          &MasterGUI::displayLogData);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::clearLogDisplay_signal, gui,
          &MasterGUI::clearLogDataDisplay);

  // Настройки
  connect(gui->ApplySettingsPushButton, &QPushButton::clicked, this,
          &GuiKernel::applySettingsPushButton_slot);
}

void GuiKernel::createProductionInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new ProductionGUI(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::createTestingInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new TestingGui(this);
  setCentralWidget(CurrentGui);

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectCurrentGui() {}

void GuiKernel::createTopMenuActions() {
  OpenMasterInterfaceAct = new QAction("Мастер доступ", this);
  OpenMasterInterfaceAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterInterfaceAct, &QAction::triggered, this,
          &GuiKernel::displayMasterInterface_slot);

  OpenAuthorizationInterfaceAct = new QAction("Авторизация", this);
  OpenAuthorizationInterfaceAct->setStatusTip("Открыть интерфейс авторизации");
  connect(OpenAuthorizationInterfaceAct, &QAction::triggered, this,
          &GuiKernel::displayAuthorizationInterface_slot);

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void GuiKernel::createTopMenu() {
  // Удаляем предыдущее топ меню
  menuBar()->clear();

  // Создаем меню
  ServiceMenu = menuBar()->addMenu("Сервис");
  switch (CurrentGui->type()) {
    case AbstractGui::Testing:
    case AbstractGui::Production:
      ServiceMenu->addAction(OpenMasterInterfaceAct);
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case AbstractGui::Master:
      ServiceMenu->addAction(OpenAuthorizationInterfaceAct);
      break;
    case AbstractGui::Authorization:
      ServiceMenu->addAction(OpenMasterInterfaceAct);
      break;
  }

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void GuiKernel::createManagersInstance() {
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
          &GuiKernel::on_RequestProductionInterface_slot);
  connect(Manager, &ClientManager::displayTransponderData_signal, this,
          &GuiKernel::displayTransponderData_slot);

  // Подключаем функционал

  connect(this, &GuiKernel::performLocalFirmwareLoading_signal, Manager,
          &ClientManager::performLocalFirmwareLoading);
  connect(this, &GuiKernel::performFirmwareReading_signal, Manager,
          &ClientManager::performFirmwareReading);
  connect(this, &GuiKernel::performFirmwareErasing_signal, Manager,
          &ClientManager::performFirmwareErasing);
  connect(this, &GuiKernel::performDataReading_signal, Manager,
          &ClientManager::performDataReading);
  connect(this, &GuiKernel::performDataLoading_signal, Manager,
          &ClientManager::performDataLoading);
  connect(this, &GuiKernel::performDeviceUnlockMemory_signal, Manager,
          &ClientManager::performDeviceUnlock);
  connect(this, &GuiKernel::performDeviceLock_signal, Manager,
          &ClientManager::performDeviceLock);

  connect(this, &GuiKernel::performLastTransponderStickerPrinting_signal,
          Manager, &ClientManager::performLastTransponderStickerPrinting);
  connect(this, &GuiKernel::performCustomTransponderStickerPrinting_signal,
          Manager, &ClientManager::performCustomTransponderStickerPrinting);
  connect(this, &GuiKernel::performStickerPrinterCommandScript_signal, Manager,
          &ClientManager::performStickerPrinterCommandScript);
  connect(this, &GuiKernel::applySettings_signal, Manager,
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

void GuiKernel::createLoggerInstance() {
  Logger = LogSystem::instance();
  connect(this, &GuiKernel::applySettings_signal, Logger,
          &LogSystem::applySettings);
  connect(this, &GuiKernel::loggerClear_signal, Logger, &LogSystem::clear);

  LoggerThread = new QThread(this);
  connect(LoggerThread, &QThread::finished, LoggerThread,
          &QThread::deleteLater);

  LogSystem::instance()->moveToThread(LoggerThread);
  LoggerThread->start();
}

void GuiKernel::createInteractorInstance() {
  Interactor = InteractionSystem::instance();
}

void GuiKernel::registerMetaTypes() {
  qRegisterMetaType<QSharedPointer<QHash<QString, QString>>>(
      "QSharedPointer<QHash<QString, QString> >");
}

void GuiKernel::on_RequestProductionInterface_slot() {
  createProductionInterface();
}

void GuiKernel::currentGuiVisibilityChanged_slot() {
  adjustSize();
  setFixedSize(size());
}

void GuiKernel::displayTransponderData_slot(
    QSharedPointer<QHash<QString, QString>> data) {
  TransponderDataModel->buildTransponderData(data.get());
  CurrentGui->update();
}
