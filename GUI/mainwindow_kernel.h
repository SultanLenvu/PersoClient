#ifndef MAINWINDOW_KERNEL_H
#define MAINWINDOW_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "Environment/client_manager.h"
#include "Environment/log_system.h"
#include "Environment/log_backend.h"
#include "Environment/text_stream_log_backend.h"
#include "Environment/widget_log_backend.h"
#include "Environment/transponder_seed_model.h"

#include "GUI/authorization_gui.h"
#include "GUI/master_gui.h"
#include "GUI/production_gui.h"
#include "GUI/user_interaction_system.h"
#include "General/definitions.h"

class MainWindowKernel : public QMainWindow {
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
  QThread* LoggerThread;

  ClientManager* Manager;
  QThread* ManagerThread;

  UserInteractionSystem* Interactor;

  TransponderInfoModel* TransponderInfo;

 public:
  MainWindowKernel();
  ~MainWindowKernel();

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

  void createManagerInstance(void);
  void createLoggerInstance(void);
  void createInteractorInstance(void);

 signals:
  void applySettings_signal(void);

  // Сигналы для логгера
  void loggerClear_signal(void);
  void loggerGenerate_signal(const QString& log);

  // Сигналы для менеджера
  void performServerConnecting_signal(void);
  void performServerDisconnecting_signal(void);
  void performServerEcho_signal(void);
  void performServerAuthorization_signal(
      const QSharedPointer<QMap<QString, QString>> data,
      bool& result);
  void performTransponderFirmwareLoading_signal(TransponderInfoModel* model);
  void performTransponderFirmwareReloading_signal(TransponderInfoModel* model,
                                                  const QString& pan);
  void performLocalFirmwareLoading_signal(const QString& path,
                                          bool unlockOption);
  void performFirmwareReading_signal(void);
  void performFirmwareErasing_signal(void);
  void performDataReading_signal(void);
  void performDataLoading_signal(const QString& path);
  void performDeviceUnlock_signal(void);
  void performDeviceLock_signal(void);
  void performPrintingLastTransponderSticker_signal(void);
  void performPrintingCustomTransponderSticker_signal(
      const QSharedPointer<QMap<QString, QString>> data);
  void performExecutingPrinterCommandScript_signal(
      const QSharedPointer<QStringList> commandScript);
};

#endif  // MAINWINDOW_KERNEL_H
