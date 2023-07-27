#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "../Environment/definitions.h"
#include "../Environment/log_system.h"
#include "../Environment/programmer_manager.h"
#include "../Environment/user_Interaction_system.h"
#include "../GUI/master_gui.h"
#include "../GUI/production_gui.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
  QRect DesktopGeometry;
  GUI *CurrentGUI;

  // Виджеты верхней панели меню
  //==================================================
  QMenu *ServiceMenu;
  QMenu *HelpMenu;

  QAction *MasterInterfaceRequestAct;
  QAction *ProductionInterfaceRequestAct;
  QAction *AboutProgramAct;
  //==================================================

  LogSystem *Logger;
  UserInteractionSystem *UserInteraction;
  ProgrammerManager *Manager;

public:
  MainWindow();
  ~MainWindow();

public slots:
  void on_AutoProgramDeviceButton_slot(void);
  void on_ManualProgramDeviceButton_slot(void);
  void on_ReadDeviceFirmwareButton_slot(void);
  void on_EraseDeviceButton_slot(void);

  void on_ProgramDeviceUserDataButton_slot(void);
  void on_ReadDeviceUserDataButton_slot(void);

  void on_UnlockDeviceButton_slot(void);
  void on_LockDeviceButton_slot(void);

  void on_MasterInterfaceRequestAct_slot(void);
  void on_ProductionInterfaceRequestAct_slot(void);

private:
  void createMasterInterface(void);
  void connectMasterInterface(void);

  void createProductionInterface(void);
  void connectProductionInterface(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

signals:
  void requestMasterPasswordFromUser(QString &pass);
  void notifyUserAboutError(const QString &text);
};

#endif // MAINWINDOW_H
