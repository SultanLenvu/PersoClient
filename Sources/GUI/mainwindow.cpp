#include "mainwindow.h"

MainWindow::MainWindow()
{
  GUI = new MainWindow_GUI(this);
  GUI->create();

  Logger = new LogSystem(this);
  connect(Logger, &LogSystem::requestDisplayLog, this, &MainWindow::displayLog);

  UserInteraction = new UserInteractionSystem(this, this);
  connect(this, &MainWindow::requestMasterPasswordFromUser, UserInteraction,
          &UserInteractionSystem::getMasterPassword);

  Manager = new FirmwareManager(this, Logger);
  connect(Manager, &FirmwareManager::logging, Logger,
          &LogSystem::loadManagerLog);
  connect(Manager, &FirmwareManager::notifyUser, UserInteraction,
          &UserInteractionSystem::firmwareManagerNotification);
}

MainWindow::~MainWindow() {}

void MainWindow::on_PB_EraseDevice_clicked() {
  GUI->GeneralLogs->clear();
  Manager->performErasing();
}

void MainWindow::on_PB_ManualProgramDevice_clicked() {

  GUI->GeneralLogs->clear();
  Manager->performLoading();
}

void MainWindow::on_PB_AutoProgramDevice_clicked() {

  GUI->GeneralLogs->clear();
  Manager->performAutoLoading();
}

void MainWindow::on_MasterAccessRequestAct_triggered() {
  QString pass;
  emit requestMasterPasswordFromUser(pass);

  if (pass == QString(MASTER_ACCESS_PASSWORD)) {
    GUI->showMasterAccessWidgets();
    Logger->enable(true);
  }
}

void MainWindow::on_CommonAccessRequestAct_triggered() {
  GUI->hideMasterAccessWidgets();
  Logger->enable(false);
}

void MainWindow::displayLog(const QString &log) {
  if (GUI->GeneralLogs->isVisible()) {
    if (GUI->GeneralLogs->toPlainText().count() > 100000)
      GUI->GeneralLogs->clear();

    GUI->GeneralLogs->appendPlainText(log);
  }
}
