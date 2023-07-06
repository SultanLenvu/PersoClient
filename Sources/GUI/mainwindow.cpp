#include "mainwindow.h"

MainWindow::MainWindow()
{
  GUI = new MainWindow_GUI(this);
  GUI->create();

  Logger = new LogSystem(this);
  connect(Logger, &LogSystem::requestDisplayLog, this, &MainWindow::displayLog);

  UserNotificator = new UserNotificationSystem(this, this);

  Manager = new LoadManager(this);
  connect(Manager, &LoadManager::logging, Logger, &LogSystem::loadManagerLog);
  connect(Manager->programmer(), &InterfaceProgrammer::logging, Logger,
          &LogSystem::programmerLog);
}

MainWindow::~MainWindow() {}

void MainWindow::on_PB_ManualProgramChip_clicked() {
  Manager->processingFirmwareLoading();
}

void MainWindow::on_PB_AutoProgramChip_clicked() {
  Manager->processingFirmwareErasing();
}

void MainWindow::displayLog(const QString &log) {
  if (GUI->GeneralLogs->toPlainText().count() > 100000)
    GUI->GeneralLogs->clear();

  GUI->GeneralLogs->appendPlainText(log);
}
