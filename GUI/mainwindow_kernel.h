#ifndef MAINWINDOW_KERNEL_H
#define MAINWINDOW_KERNEL_H

#include <QMainWindow>
#include <QSettings>
#include <QtWidgets>

#include "Environment/client_manager.h"
#include "Environment/log_system.h"
#include "Environment/transponder_seed_model.h"

#include "GUI/authorization_gui.h"
#include "GUI/master_gui.h"
#include "GUI/production_gui.h"
#include "GUI/user_interaction_system.h"
#include "General/definitions.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
  QRect DesktopGeometry;
  GUI* CurrentGUI;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QMenu* HelpMenu;

  QAction* MasterInterfaceRequestAct;
  QAction* ProductionInterfaceRequestAct;
  QAction* ExitFromProductionLineAct;
  QAction* AboutProgramAct;
  //==================================================

  LogSystem* Logger;
  UserInteractionSystem* Interactor;
  ClientManager* Manager;

  TransponderInfoModel* TransponderInfo;

 public:
  MainWindow();
  ~MainWindow();

 public slots:
  void on_AuthorizePushButton_slot(void);

  // Сервер
  void on_PersoServerConnectPushButton_slot(void);
  void on_PersoServerDisconnectButton_slot(void);
  void on_PersoServerEchoRequestButton_slot(void);
  void on_MasterAuthorizePushButton_slot(void);
  void on_LoadTransponderFirmwareButton_slot(void);
  void on_ReloadTransponderFirmwareButton_slot(void);

  // Программатор
  void on_ProgramDeviceButton_slot(void);
  void on_ReadDeviceFirmwareButton_slot(void);
  void on_EraseDeviceButton_slot(void);
  void on_ProgramDeviceUserDataButton_slot(void);
  void on_ReadDeviceUserDataButton_slot(void);
  void on_UnlockDeviceButton_slot(void);
  void on_LockDeviceButton_slot(void);

  // Стикер принтер
  void on_PrintLastTransponderStickerButton_slot(void);
  void on_PrintCustomTransponderStickerButton_slot(void);
  void on_ExecuteStickerPrinterCommandScriptButton_slot(void);

  // Настройки
  void on_ApplySettingsPushButton_slot(void);

  // Верхнее меню
  void on_MasterInterfaceRequest_slot(void);
  void on_ProductionInterfaceRequest_slot(void);
  void on_ExitFromProductionLineAct_slot(void);

 private:
  void proxyLogging(const QString& log);
  void loadSettings(void);
  bool checkNewSettings(void);
  QString getStickerPan(QStringList& stickerData);

  void createAuthorizationInterface(void);
  void connectAuthorizationInterface(void);

  void createMasterInterface(void);
  void connectMasterInterface(void);

  void createProductionInterface(void);
  void connectProductionInterface(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createManager(void);
};

#endif  // MAINWINDOW_KERNEL_H
