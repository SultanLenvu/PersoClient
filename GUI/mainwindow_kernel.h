#ifndef MAINWINDOW_KERNEL_H
#define MAINWINDOW_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "Environment/client_manager.h"
#include "GUI/authorization_gui.h"
#include "GUI/master_gui.h"
#include "GUI/production_gui.h"
#include "GUI/user_interaction_system.h"
#include "General/definitions.h"
#include "General/string_map_model.h"
#include "Log/log_system.h"

class MainWindowKernel : public QMainWindow {
  Q_OBJECT

 private:
  QRect DesktopGeometry;
  GUI* CurrentGUI;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QMenu* HelpMenu;

  QAction* OpenMasterInterfaceAct;
  QAction* OpenProductionInterfaceAct;
  QAction* OpenAuthorizationInterfaceAct;
  QAction* AboutProgramAct;
  //==================================================

  QThread* LoggerThread;

  ClientManager* Manager;
  QThread* ManagerThread;

  UserInteractionSystem* Interactor;

  StringMapModel* TransponderInfo;

 public:
  explicit MainWindowKernel(QWidget* parent = nullptr);
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
  void on_OpenAuthorizationInterfaceAct_slot(void);

 private:
  Q_DISABLE_COPY(MainWindowKernel);
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

 private slots:
  void on_RequestProductionInterface_slot(void);

 signals:
  void applySettings_signal(void);

  // Сигналы для логгера
  void loggerClear_signal(void);

  // Сигналы для менеджера
  void performServerConnecting_signal(void);
  void performServerDisconnecting_signal(void);
  void performServerEcho_signal(void);
  void performServerAuthorization_signal(
      const QSharedPointer<QMap<QString, QString>> data);
  void performTransponderFirmwareLoading_signal(StringMapModel* model);
  void performTransponderFirmwareReloading_signal(StringMapModel* model,
                                                  const QString& pan);
  void performLocalFirmwareLoading_signal(const QString& path);
  void performFirmwareReading_signal(void);
  void performFirmwareErasing_signal(void);
  void performDataReading_signal(void);
  void performDataLoading_signal(const QString& path);
  void performDeviceUnlock_signal(void);
  void performDeviceLock_signal(void);
  void performPrintingLastTransponderSticker_signal(void);
  void performPrintingCustomTransponderSticker_signal(
      const QSharedPointer<QMap<QString, QString>> data);
  void performStickerPrinterCommandScript_signal(
      const QSharedPointer<QStringList> commandScript);
};

#endif  // MAINWINDOW_KERNEL_H
