#include "mainwindow.h"

MainWindow::MainWindow() {
  // Логгер пока не создан
  Logger = nullptr;

  // Интерфейс пока не создан
  CurrentGUI = nullptr;

  // Система для взаимодействия с пользователем
  UserInteraction = new UserInteractionSystem(this, this);
  connect(this, &MainWindow::requestMasterPasswordFromUser, UserInteraction,
          &UserInteractionSystem::getMasterPassword);
  connect(this, &MainWindow::notifyUserAboutError, UserInteraction,
          &UserInteractionSystem::generateErrorMessage);

  // Менеджер для взаимодействия с программатором
  Manager = new FirmwareManager(this);
  connect(Manager, &FirmwareManager::notifyUser, UserInteraction,
          &UserInteractionSystem::firmwareManagerNotification);

  // Настраиваем размер главного окна
  DesktopGeometry = QApplication::desktop()->screenGeometry();
  setGeometry(DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
              DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  setLayoutDirection(Qt::LeftToRight);

  // Создаем графический интерфейс
  createProductionInterface();
}

MainWindow::~MainWindow() {}

void MainWindow::on_EraseDevice_slot() {
  dynamic_cast<MasterGUI *>(CurrentGUI)->GeneralLogs->clear();

  Manager->performErasing();
}

void MainWindow::on_ProgramDevice_slot() {
  if (CurrentGUI->type() == GUI::Master)
    dynamic_cast<MasterGUI *>(CurrentGUI)->GeneralLogs->clear();

  Manager->performLoading();
}

void MainWindow::on_MasterInterfaceRequestAct_slot() {
  createMasterInterface();

  Logger = new LogSystem(this);
  connect(Logger, &LogSystem::requestDisplayLog,
          dynamic_cast<MasterGUI *>(CurrentGUI), &MasterGUI::displayLogData);
  connect(Manager, &FirmwareManager::logging, Logger,
          &LogSystem::loadManagerLog);
  Manager->setLogger(Logger);
}

void MainWindow::on_ProductionInterfaceRequestAct_slot() {
  createProductionInterface();

  delete Logger;
  Logger = nullptr;
}

void MainWindow::createMasterInterface() {
  QString pass;
  emit requestMasterPasswordFromUser(pass);

  if (pass == QString(MASTER_ACCESS_PASSWORD)) {
    // Создаем интерфейс
    delete CurrentGUI;
    CurrentGUI = new MasterGUI(this);
    setCentralWidget(CurrentGUI->create());

    // Подключаем интерфейс
    connectMasterInterface();

    // Создаем верхнее меню
    createTopMenu();
  } else
    emit notifyUserAboutError("Неверный пароль");
}

void MainWindow::connectMasterInterface() {
  MasterGUI *gui = dynamic_cast<MasterGUI *>(CurrentGUI);

  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_ProgramDevice_slot);
  connect(gui->EraseDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_EraseDevice_slot);
}

void MainWindow::createProductionInterface() {
  // Создаем интерфейс
  delete CurrentGUI;
  CurrentGUI = new ProductionGUI(this);
  setCentralWidget(CurrentGUI->create());

  // Подключаем интерфейс
  connectProductionInterface();

  // Создаем верхнее меню
  createTopMenu();
}

void MainWindow::connectProductionInterface() {
  ProductionGUI *gui = dynamic_cast<ProductionGUI *>(CurrentGUI);

  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &MainWindow::on_ProgramDevice_slot);
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
