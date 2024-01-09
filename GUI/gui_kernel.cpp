#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "authorization_gui.h"
#include "definitions.h"
#include "gui_kernel.h"
#include "master_gui.h"
#include "master_password_input_dialog.h"
#include "production_assembler_gui.h"
#include "production_gui_subkernel.h"
#include "production_manager.h"
#include "production_tester_gui.h"
#include "programmer_gui_subkernel.h"
#include "programmer_manager.h"
#include "sticker_printer_gui_subkernel.h"
#include "sticker_printer_manager.h"
#include "widget_log_backend.h"

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

  // Создаем подядра для обработки пользовательских сигналов
  createGuiSubkernels();

  // Создаем графический интерфейс для авторизации
  createAuthorizationGui();

  // Регистрируем типы
  registerMetaTypes();
}

GuiKernel::~GuiKernel() {
  ManagersThread->exit();
  ManagersThread->wait();

  ServiceThread->exit();
  ServiceThread->wait();
}

void GuiKernel::applySettingsPushButton_slot() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui.get());
  QSettings settings;

  emit clearLogDisplay_signal();

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

  settings.setValue("perso_server_connection/ip",
                    gui->PersoServerIpAddressLineEdit->text());
  settings.setValue("perso_server_connection/port",
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

void GuiKernel::displayAuthorizationGui_slot() {
  createAuthorizationGui();
}

void GuiKernel::loadSettings() {
  QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
  QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
  QCoreApplication::setApplicationName(PROGRAM_NAME);

  QSettings::setDefaultFormat(QSettings::IniFormat);
  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                     QCoreApplication::applicationDirPath());
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
  connect(this, &GuiKernel::clearLogDisplay_signal, Logger.get(),
          &LogSystem::clear);

  ServiceThread = std::unique_ptr<QThread>(new QThread());

  Logger->moveToThread(ServiceThread.get());
  ServiceThread->start();
}

void GuiKernel::createInteractorInstance() {
  Interactor = std::unique_ptr<InteractionSystem>(
      new InteractionSystem("InteractionSystem"));
}

void GuiKernel::createManagersInstance() {
  Managers["ProductionManager"] = std::unique_ptr<ProductionManager>(
      new ProductionManager("ProductionManager"));
  Managers["ProgrammerManager"] = std::unique_ptr<ProgrammerManager>(
      new ProgrammerManager("ProgrammerManager"));
  Managers["StickerPrinterManager"] = std::unique_ptr<StickerPrinterManager>(
      new StickerPrinterManager("StickerPrinterManager"));

  // Создаем отдельный поток
  ManagersThread = std::unique_ptr<QThread>(new QThread());

  for (auto it = Managers.cbegin(); it != Managers.cend(); it++) {
    connect(it->second.get(), &AbstractManager::logging, Logger.get(),
            &LogSystem::generate);
    connect(it->second.get(), &AbstractManager::executionStarted,
            Interactor.get(), &InteractionSystem::operationStarted);
    connect(it->second.get(), &AbstractManager::executionStarted,
            Interactor.get(), &InteractionSystem::operationFinished);

    it->second.get()->moveToThread(ManagersThread.get());
  }

  ManagersThread->start();
}

void GuiKernel::createAuthorizationGui() {
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
  connectAuthorizationGui();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectAuthorizationGui() {
  for (auto it = Subkernels.cbegin(); it != Subkernels.cend(); it++) {
    it->second->connectAuthorizationGui(
        dynamic_cast<std::shared_ptr<AuthorizationGui>>(CurrentGui));
  }
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
  connectMasterInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectMasterGui() {}

void GuiKernel::connectMasterInterface() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::displayLog_signal, gui,
          &MasterGui::displayLogData);
  connect(LogSystem::instance()->getWidgetLogger(),
          &WidgetLogBackend::clearLogDisplay_signal, gui,
          &MasterGui::clearLogDataDisplay);

  // Настройки
  connect(gui->ApplySettingsPushButton, &QPushButton::clicked, this,
          &GuiKernel::applySettingsPushButton_slot);
}

void GuiKernel::createProductionAssemblerGui() {
  // Настраиваем размер главного окна
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new ProductionAssemblerGui(this);
  setCentralWidget(CurrentGui);

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectProductionAssemblerGui() {}

void GuiKernel::createProductionTesterGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  CurrentGui = new ProductionTesterGui(this);
  setCentralWidget(CurrentGui);

  // Создаем верхнее меню
  createTopMenu();
}

void GuiKernel::connectProductionTesterGui() {}

void GuiKernel::createTopMenuActions() {
  OpenMasterInterfaceAct = new QAction("Мастер доступ", this);
  OpenMasterInterfaceAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterInterfaceAct, &QAction::triggered, this,
          &GuiKernel::displayMasterGui_slot);

  OpenAuthorizationInterfaceAct = new QAction("Авторизация", this);
  OpenAuthorizationInterfaceAct->setStatusTip("Открыть интерфейс авторизации");
  connect(OpenAuthorizationInterfaceAct, &QAction::triggered, this,
          &GuiKernel::displayAuthorizationGui_slot);

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
  connect(subkernel.get(),
          &ProductionGuiSubkernel::displayProductionAssemblerGui, this,
          &GuiKernel::displayProductionTesterGui_slot);

  subkernel->connectManager(
      dynamic_cast<ProductionManager*>(Managers["ProductionManager"].get()));

  Subkernels["ProductionGuiSubkernel"] = subkernel;
}

void GuiKernel::createProgrammerGuiSubkernel() {
  std::shared_ptr<ProgrammerGuiSubkernel> subkernel(
      new ProgrammerGuiSubkernel("ProgrammerGuiSubkernel"));

  subkernel->connectManager(
      dynamic_cast<ProgrammerManager*>(Managers["ProgrammerManager"].get()));

  Subkernels["ProgrammerGuiSubkernel"] = subkernel;
}

void GuiKernel::createStickerPrinterGuiSubkernel() {
  std::shared_ptr<StickerPrinterGuiSubkernel> subkernel(
      new StickerPrinterGuiSubkernel("StickerPrinterGuiSubkernel"));

  subkernel->connectManager(dynamic_cast<StickerPrinterManager*>(
      Managers["StickerPrinterManager"].get()));

  Subkernels["StickerPrinterGuiSubkernel"] = subkernel;
}

bool GuiKernel::checkNewSettings() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui.get());

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
