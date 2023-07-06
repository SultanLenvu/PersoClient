#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "../Environment/load_manager.h"
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
  LoadManager *Manager;

public:
  MainWindow();
  ~MainWindow();

public slots:
  void on_PB_ManualProgramChip_clicked(void);
  void on_PB_AutoProgramChip_clicked(void);

public slots:
  void displayLog(const QString &log);
};

#endif // MAINWINDOW_H
