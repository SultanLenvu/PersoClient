#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "../Environment/definitions.h"
#include "../Environment/firmware_manager.h"
#include "../Environment/log_system.h"
#include "../Environment/user_Interaction_system.h"
#include "../GUI/mainwindow_gui.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  MainWindow_GUI *GUI;
  LogSystem *Logger;
  UserInteractionSystem *UserInteraction;
  FirmwareManager *Manager;

public:
  MainWindow();
  ~MainWindow();

public slots:
  void on_PB_EraseDevice_clicked(void);
  void on_PB_ManualProgramDevice_clicked(void);
  void on_PB_AutoProgramDevice_clicked(void);
  void on_MasterAccessRequestAct_triggered(void);
  void on_CommonAccessRequestAct_triggered(void);

public slots:
  void displayLog(const QString &log);

signals:
  void requestMasterPasswordFromUser(QString &pass);
};

#endif // MAINWINDOW_H
