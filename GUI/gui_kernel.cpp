#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

#include "assembler_unit_user_interface.h"
#include "authorization_user_interface.h"
#include "business_object_environment.h"
#include "definitions.h"
#include "gui_kernel.h"
#include "master_access_dialog.h"
#include "master_user_interface.h"
#include "production_manager_gui_subkernel.h"
#include "programmer_gui_subkernel.h"
#include "server_connection_gui_subkernel.h"
#include "service_object_environment.h"
#include "settings_dialog.h"
#include "sticker_printer_gui_subkernel.h"
#include "tester_unit_user_interface.h"

GuiKernel::GuiKernel(QWidget* parent)
    : QMainWindow(parent), CurrentMode(Authorization) {
  DesktopGeometry = QApplication::primaryScreen()->size();
  createTopMenu();

  createEnvironments();
  createReactions();
  createGuiSubkernels();

  // Создаем графический интерфейс для авторизации
  //  createAuthorizationUserInterface();
  createMasterInterface();
}

void GuiKernel::displayMasterInterface() {
  StringDictionary param;
  MasterAccessDialog dialog(nullptr);
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
  createMasterInterface();
}

void GuiKernel::displayAssemblerUnitInterface() {
  createAssemblerUnitUserInterface();
}

void GuiKernel::displayTesterUnitInterface() {
  createProductionTesterGui();
}

void GuiKernel::logOutServerAct_slot() {
  emit logOutServer_signal();

  createAuthorizationUserInterface();
}

void GuiKernel::createReactions() {
  PIndicator = std::make_unique<ProgressIndicator>("ProgressIndicator");
  SIndicator = std::make_unique<StatusIndicator>("StatusIndicator");
}

void GuiKernel::createAuthorizationUserInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.1);
  setLayoutDirection(Qt::LeftToRight);
  setMinimumWidth(DesktopGeometry.width() * 0.2);

  // Создаем интерфейс
  setCentralWidget(new AuthorizationUserInterface());

  adjustSize();
  setFixedSize(size());
}

void GuiKernel::createMasterInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем интерфейс
  setCentralWidget(new MasterUserInterface());
}

void GuiKernel::createAssemblerUnitUserInterface() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  setCentralWidget(new AssemblerUnitUserInterface());
}

void GuiKernel::createProductionTesterGui() {
  // Настраиваем размер главного окна
  setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.8, DesktopGeometry.height() * 0.8);
  setLayoutDirection(Qt::LeftToRight);

  setCentralWidget(new TesterUnitUserInterface());
}

void GuiKernel::displaySettingsDialog() {
  SettingsDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
}

void GuiKernel::createTopMenuActions() {
  OpenMasterInterfaceAct = new QAction("Мастер доступ", this);
  OpenMasterInterfaceAct->setStatusTip("Открыть мастер интерфейс");
  connect(OpenMasterInterfaceAct, &QAction::triggered, this,
          &GuiKernel::displayMasterInterface);

  LogOutServerAct = new QAction("Авторизация", this);
  LogOutServerAct->setStatusTip("Открыть интерфейс авторизации");
  connect(LogOutServerAct, &QAction::triggered, this,
          &GuiKernel::logOutServerAct_slot);

  SettingsDialogAction = new QAction("Настройки", this);
  SettingsDialogAction->setStatusTip("Открыть интерфейс настроек");
  connect(SettingsDialogAction, &QAction::triggered, this,
          &GuiKernel::displaySettingsDialog);

  AboutProgramAct = new QAction("О программе", this);
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void GuiKernel::createTopMenu() {
  createTopMenuActions();
  menuBar()->clear();

  ServiceMenu = menuBar()->addMenu("Сервис");

  ServiceMenu->addAction(OpenMasterInterfaceAct);
  ServiceMenu->addAction(LogOutServerAct);
  ServiceMenu->addAction(SettingsDialogAction);

  HelpMenu = menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void GuiKernel::createEnvironments() {
  std::unique_ptr<AsyncEnvironment> ae1 = std::make_unique<AsyncEnvironment>();
  ae1->add<BusinessObjectEnvironment>(Subkernels);

  std::unique_ptr<AsyncEnvironment> ae2 = std::make_unique<AsyncEnvironment>();
  ae2->add<ServiceObjectEnvironment>();

  Environments.push_back(std::move(ae1));
  Environments.push_back(std::move(ae2));
}

void GuiKernel::createGuiSubkernels() {
  Subkernels.emplace(
      "ProductionManagerGuiSubkernel",
      new ProductionManagerGuiSubkernel("ProductionManagerGuiSubkernel"));
  Subkernels.emplace("ProductionManagerGuiSubkernel",
                     new ProgrammerGuiSubkernel("ProgrammerGuiSubkernel"));
  Subkernels.emplace(
      "ProductionManagerGuiSubkernel",
      new StickerPrinterGuiSubkernel("StickerPrinterGuiSubkernel"));
  Subkernels.emplace(
      "ProductionManagerGuiSubkernel",
      new ServerConnectionGuiSubkernel("ServerConnectionGuiSubkernel"));
}
