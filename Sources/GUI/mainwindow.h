#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "../Environment/firmware_manager.h"
#include "../Environment/log_system.h"
#include "../Environment/notification_system.h"
#include "../GUI/mainwindow_gui.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  MainWindow_GUI *GUI;
  LogSystem *Logger;
  UserNotificationSystem *UserNotificator;
  FirmwareManager *Manager;

public:
  MainWindow();
  ~MainWindow();

public slots:
  void on_PB_EraseDevice_clicked(void);
  void on_PB_ManualProgramDevice_clicked(void);
  void on_PB_AutoProgramDevice_clicked(void);

public slots:
  void displayLog(const QString &log);
};

#endif // MAINWINDOW_H
