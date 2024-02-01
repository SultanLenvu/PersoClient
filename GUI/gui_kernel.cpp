#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "authorization_gui.h"
#include "definitions.h"
#include "global_environment.h"
#include "gui_kernel.h"
#include "master_gui.h"
#include "master_password_input_dialog.h"
#include "production_assembler_gui.h"
#include "production_gui_subkernel.h"
#include "production_manager.h"
#include "production_tester_gui.h"
#include "programmer_gui_subkernel.h"
#include "programmer_manager.h"
#include "settings_dialog.h"
#include "sticker_printer_gui_subkernel.h"
#include "sticker_printer_manager.h"

GuiKernel::GuiKernel(QWidget* parent) : QMainWindow(parent) {
  setObjectName("GuiKernel");
  CurrentGui = nullptr;
  loadSettings();
  DesktopGeometry = QApplication::primaryScreen()->size();

  /* !!!
   * Порядок создания сущностей критически важен
   * !!!
   */

  // Глобальная среда для соединения объектов
  GlobalEnv = GlobalEnvironment::instance();
  GlobalEnv->registerObject(this);

  // Создаем действия для верхнего меню
  createTopMenuActions();

  // Создаем логгер
  createLoggerInstance();

  // Система для взаимодействия с пользователем
  createInteractorInstance();

  // Менеджер для взаимодействия с программатором
  createManagersInstance();

  // Создаем подядра для обработки пользовательских сигналов
  createGuiSubkernels();

  // Создаем графический интерфейс для авторизации
  //  createAuthorizationGui();
  createMasterGui();

  // Регистрируем типы
  registerMetaTypes();

  GlobalEnvironment::instance()->registerObject(this);
}

GuiKernel::~GuiKernel() {
  ManagersThread->exit();
  ManagersThread->wait();

  ServiceThread->exit();
  ServiceThread->wait();
}

void GuiKernel::applySettings() {
  sendLog("Применение новых настроек. ");
  emit applySettings_signal();
}

void GuiKernel::displayMasterGui_slot() {
  StringDictionary param;
  MasterPasswordInputDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  if (param.value("password") != QString(MASTER_ACCESS_PASSWORD)) {
    QMessageBox::critical(nullptr, "Ошибка", "Неверный пароль",
                          QMessageBox::Ok);
    return;
  }

  // Создаем мастер интерфейс
  createMasterGui();
}

void GuiKernel::displayProductionAssemblerGui_slot() {
  createProductionAssemblerGui();
}

void GuiKernel::displayProductionTesterGui_slot() {
  createProductionTesterGui();
}

void GuiKernel::logOutServerAct_slot() {
  emit logOutServer_signal();

  createAuthorizationGui();
}

void GuiKernel::displaySettingsDialog_slot() {
  SettingsDialog dialog(this);
  connect(&dialog, &SettingsDialog::applyNewSettings, this,
          &GuiKernel::applySettings);

  dialog.exec();
}

void GuiKernel::onServerDisconnected() {
  if ((CurrentGui->type() == AbstractGui::ProductionAssembler) ||
      (CurrentGui->type() == AbstractGui::ProductionTester)) {
    Interactor->generateErrorMessage("Соединение с сервером оборвалось.");

    delete CurrentGui;
    createAuthorizationGui();
  } else {
    Interactor->generateErrorMessage("Соединение с сервером оборвалось.");
  }
}

void GuiKernel::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

void GuiKernel::loadSettings() {
  QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(PROGRAM_NAME);

  QSettings::setDefaultFormat(QSettings::IniFormat);
}

void GuiKernel::registerMetaTypes() {
  qRegisterMetaType<std::shared_ptr<StringDictionary>>(
      "std::shared_ptr<StringDictionary>");
  qRegisterMetaType<std::shared_ptr<QStringList>>(
      "std::shared_ptr<QStringList>");
  qRegisterMetaType<std::shared_ptr<QString>>("std::shared_ptr<QString>");
}

void GuiKernel::createLoggerInstance() {
  Logger = std::unique_ptr<LogSystem>(new LogSystem("LogSystem"));
  connect(this, &GuiKernel::logging, Logger.get(), &LogSystem::generate);
  connect(this, &GuiKernel::clearLogDisplay_signal, Logger.get(),
          &LogSystem::clear);
  connect(this, &GuiKernel::applySettings_signal, Logger.get(),
          &LogSystem::applySettings);

  ServiceThread = std::unique_ptr<QThread>(new QThread());

  Logger->moveToThread(ServiceThread.get());
  ServiceThread->start();
}

void GuiKernel::createInteractorInstance() {
  Interactor = std::unique_ptr<InteractionSystem>(
      new InteractionSystem("InteractionSystem"));
  connect(this, &GuiKernel::applySettings_signal, Interactor.get(),
          &InteractionSystem::applySettings);
}

void GuiKernel::createManagersInstance() {
  std::shared_ptr<ProductionManager> pm(
      new ProductionManager("ProductionManager"));
  connect(this, &GuiKernel::logOutServer_signal, pm.get(),
          &ProductionManager::logOutServer);

  Managers["ProductionManager"] = pm;
  Managers["ProgrammerManager"] = std::shared_ptr<ProgrammerManager>(
      new ProgrammerManager("ProgrammerManager"));
  Managers["StickerPrinterManager"] = std::shared_ptr<StickerPrinterManager>(
      new StickerPrinterManager("StickerPrinterManager"));

  // Создаем отдельный поток
  ManagersThread = std::unique_ptr<QThread>(new QThread());

  for (auto it = Managers.cbegin(); it != Managers.cend(); it++) {
    connect(this, &GuiKernel::applySettings_signal, it->second.get(),
            &AbstractManager::applySettings);

    connect(ManagersThread.get(), &QThread::started, it->second.get(),
            &AbstractManager::onInstanceThreadStarted);

    it->second.get()->moveToThread(ManagersThread.get());
  }

  ManagersThread->start();

  connect(static_cast<ProductionManager*>(Managers["ProductionManager"].get()),
          &ProductionManager::printTransponderSticker_signal,
          static_cast<StickerPrinterManager*>(
              Managers["StickerPrinterManager"].get()),
          &StickerPrinterManager::printTransponderSticker_sync,
          Qt::DirectConnection);
}

void GuiKernel::createAuthorizationGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.1);
  setLayoutDirection(Qt::LeftToRight);
  setMinimumWidth(DesktopGeometry.width() * 0.2);

  // Создаем интерфейс
  CurrentGui = new AuthorizationGui(this);
  setCentralWidget(CurrentGui);
  CurrentGui->showMaximized();

  adjustSize();
  setFixedSize(size());

  // Подключаем интерфейс
  connectGui();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::createMasterGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  CurrentGui = new MasterGui(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectGui();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::createProductionAssemblerGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new ProductionAssemblerGui(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectGui();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::createProductionTesterGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new ProductionTesterGui(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectGui();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectGui() {
  for (auto it = Subkernels.cbegin(); it != Subkernels.cend(); it++) {
    it->second->connectGui(CurrentGui);
  }
}

void GuiKernel::createTopMenuActions() {
  OpenMasterGuiAct = new QAction("Мастер доступ", this);
  OpenMasterGuiAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterGuiAct, &QAction::triggered, this,
          &GuiKernel::displayMasterGui_slot);

  LogOutServerAct = new QAction("Авторизация", this);
  LogOutServerAct->setStatusTip("Открыть интерфейс авторизации");
  connect(LogOutServerAct, &QAction::triggered, this,
          &GuiKernel::logOutServerAct_slot);

  OpenSettingsDialogAct = new QAction("Настройки", this);
  OpenSettingsDialogAct->setStatusTip("Открыть интерфейс настроек");
  connect(OpenSettingsDialogAct, &QAction::triggered, this,
          &GuiKernel::displaySettingsDialog_slot);

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void GuiKernel::createTopMenu() {
  // Удаляем предыдущее топ меню
  menuBar()->clear();

  // Создаем меню
  ServiceMenu = menuBar()->addMenu("Сервис");
  switch (CurrentGui->type()) {
    case AbstractGui::ProductionTester:
    case AbstractGui::ProductionAssembler:
      ServiceMenu->addAction(OpenMasterGuiAct);
      ServiceMenu->addAction(LogOutServerAct);
      break;
    case AbstractGui::Master:
      ServiceMenu->addAction(LogOutServerAct);
      break;
    case AbstractGui::Authorization:
      ServiceMenu->addAction(OpenMasterGuiAct);
      break;
  }
  ServiceMenu->addAction(OpenSettingsDialogAct);

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void GuiKernel::createGuiSubkernels() {
  createProductionGuiSubkernel();
  createProgrammerGuiSubkernel();
  createStickerPrinterGuiSubkernel();
}

void GuiKernel::createProductionGuiSubkernel() {
  std::shared_ptr<ProductionGuiSubkernel> subkernel(
      new ProductionGuiSubkernel("ProductionGuiSubkernel"));

  connect(subkernel.get(),
          &ProductionGuiSubkernel::displayProductionAssemblerGui, this,
          &GuiKernel::displayProductionAssemblerGui_slot);
  connect(subkernel.get(), &ProductionGuiSubkernel::displayProductionTesterGui,
          this, &GuiKernel::displayProductionTesterGui_slot);

  subkernel->connectManager(Managers["ProductionManager"]);

  Subkernels["ProductionGuiSubkernel"] = subkernel;
}

void GuiKernel::createProgrammerGuiSubkernel() {
  std::shared_ptr<ProgrammerGuiSubkernel> subkernel(
      new ProgrammerGuiSubkernel("ProgrammerGuiSubkernel"));

  subkernel->connectManager(Managers["ProgrammerManager"]);

  Subkernels["ProgrammerGuiSubkernel"] = subkernel;
}

void GuiKernel::createStickerPrinterGuiSubkernel() {
  std::shared_ptr<StickerPrinterGuiSubkernel> subkernel(
      new StickerPrinterGuiSubkernel("StickerPrinterGuiSubkernel"));

  subkernel->connectManager(Managers["StickerPrinterManager"]);

  Subkernels["StickerPrinterGuiSubkernel"] = subkernel;
}
