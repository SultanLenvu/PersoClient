#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtWidgets>

#include "Environment/firmware_manager.h"
#include "Environment/log_system.h"
#include "Environment/user_interaction_system.h"
#include "GUI/authorization_gui.h"
#include "GUI/master_gui.h"
#include "GUI/production_gui.h"
#include "General/definitions.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
  QRect DesktopGeometry;
  GUI* CurrentGUI;
  QSettings* Settings;

  // Виджеты верхней панели меню
  //==================================================
  QMenu* ServiceMenu;
  QMenu* HelpMenu;

  QAction* MasterInterfaceRequestAct;
  QAction* ProductionInterfaceRequestAct;
  QAction* AboutProgramAct;
  //==================================================

  LogSystem* Logger;
  UserInteractionSystem* Interactor;
  FirmwareManager* Manager;

 public:
  MainWindow();
  ~MainWindow();

 public slots:
  void on_AuthorizePushButton_slot(void);

  void on_PersoServerConnectPushButton_slot(void);
  void on_PersoServerDisconnectButton_slot(void);
  void on_PersoServerEchoRequestButton_slot(void);
  void on_PersoServerFirmwareRequestButton_slot(void);

  void on_ServerProgramDeviceButton_slot(void);
  void on_ManualProgramDeviceButton_slot(void);
  void on_ReadDeviceFirmwareButton_slot(void);
  void on_EraseDeviceButton_slot(void);

  void on_ProgramDeviceUserDataButton_slot(void);
  void on_ReadDeviceUserDataButton_slot(void);

  void on_UnlockDeviceButton_slot(void);
  void on_LockDeviceButton_slot(void);

  void on_ApplySettingsPushButton_slot(void);

  void on_MasterInterfaceRequestAct_slot(void);
  void on_ProductionInterfaceRequestAct_slot(void);

 private:
  void proxyLogging(const QString& log);
  void loadSettings(void);
  bool checkNewSettings(void);

  void createMasterInterface(void);
  void connectMasterInterface(void);

  void createAuthorizationInterface(void);
  void connectAuthorizationInterface(void);

  void createProductionInterface(void);
  void connectProductionInterface(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createManager(void);
  void createInteractor(void);
  void createLogger(void);

 signals:
  void logging(const QString& log);
  void requestMasterPasswordFromUser(QString& pass);
  void notifyUserAboutError(const QString& text);
  void notifyUser(const QString& text);
};

#endif  // MAINWINDOW_H
