#include "mainwindow.h"

MainWindow::MainWindow()
{
  GUI = new MainWindow_GUI(this);
  GUI->create();

  Logger = new LogSystem(this);
  connect(Logger, &LogSystem::requestDisplayLog, this, &MainWindow::displayLog);

  UserNotificator = new UserNotificationSystem(this, this);

  Manager = new FirmwareManager(this, Logger);
  connect(Manager, &FirmwareManager::logging, Logger,
          &LogSystem::loadManagerLog);
  connect(Manager, &FirmwareManager::notifyUser, UserNotificator,
          &UserNotificationSystem::generateUserNotification);
}

MainWindow::~MainWindow() {}

void MainWindow::on_PB_EraseDevice_clicked() {
  // GUI->GeneralLogs->clear();
  Manager->performErasing();
}

void MainWindow::on_PB_ManualProgramDevice_clicked() {
  // GUI->GeneralLogs->clear();
  Manager->performLoading();
}

void MainWindow::on_PB_AutoProgramDevice_clicked() {
  // GUI->GeneralLogs->clear();
  Manager->performAutoLoading();
}

void MainWindow::displayLog(const QString &log) {
  if (GUI->GeneralLogs->toPlainText().count() > 100000)
    GUI->GeneralLogs->clear();

  GUI->GeneralLogs->appendPlainText(log);
}
