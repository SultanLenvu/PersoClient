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
#include "GUI/interaction_system.h"
#include "GUI/master_gui.h"
#include "GUI/production_gui.h"
#include "General/definitions.h"
#include "General/map_model.h"
#include "Log/log_system.h"

/*!
 * Main window, which gets created in main.cpp
 * Holds loggers, client manager, and user interaction system.
 */
class MainWindowKernel : public QMainWindow {
  Q_OBJECT

 private:
  //! \cond
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
  //! \endcond

  //! Logger
  LogSystem* Logger;
  //! Thread where logger runs
  QThread* LoggerThread;

  //! Client manager
  ClientManager* Manager;
  //! Thread where ClientManager runs
  QThread* ManagerThread;

  //! Instance of InteractionSystem
  InteractionSystem* Interactor;

  //! Transponder data
  MapModel* TransponderInfo;

 public:
  /*!
   * Default constructor
   * \param[in] parent QObject parent
   */
  explicit MainWindowKernel(QWidget* parent = nullptr);
  /*!
   * Default destructor
   */
  ~MainWindowKernel();

 public slots:
  //! \cond
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
  //! \endcond

 private:
  //! \cond
  Q_DISABLE_COPY(MainWindowKernel);
  //! \endcond
  /*!
   * Load settings from QSettings
   */
  void loadSettings(void);
  /*! Validate settings
   * Checks whether:
   * - Server IP address is valid;
   * - Server port is in range [IP_PORT_MIN_VALUE; IP_PORT_MAX_VALUE]
   * - JLink path points to existent and executable file
   * - Speed is not zero
   * - TSCLib path points to existent file with '.dll' extension
   * - Log system save path points to a directory
   * \return true if these conditions hold, false otherwise
   */
  bool checkNewSettings(void);
  /*!
   * Get PAN from sticker data
   * \param[in] stickerData data to extract PAN from
   * \return PAN if stickerData has 2 elements or less and if there's
   * a string of length PAN_CHAR_LENGTH, empty string otherwise
   */
  QString getStickerPan(QStringList& stickerData);

  /*!
   * Create widgets and layout of authorization interface
   */
  void createAuthorizationInterface(void);
  /*!
   * Connect authorization interface signals to slots
   */
  void connectAuthorizationInterface(void);

  /*!
   * Create widgets and layout of master interface
   */
  void createMasterInterface(void);
  /*!
   * Connect master interface signals to slots
   */
  void connectMasterInterface(void);

  /*!
   * Create widgets and layout of production interface
   */
  void createProductionInterface(void);
  /*!
   * Connect production interface signals to slots
   */
  void connectProductionInterface(void);

  /*!
   * Create QActions for top menu
   */
  void createTopMenuActions(void);
  /*!
   * Create top menu depending on which interface is active
   * right now
   */
  void createTopMenu(void);

  /*!
   * create ClientManager instance and connect signals to slots:
   * - Manager->logging -> Logger->generate
   * - Manager->notifyUser -> Interactor->generateMessage
   * - Manager->notifyUserAboutError -> Interactor->generateErrorMessage
   * - Manager->operationPerformingStarted 
   *   -> Interactor->startOperationProgressDialog
   * - Manager->operationPerformingFinished
   *   -> Interactor->finishOperationProgressDialog
   * - Manager->requestProductionInterface_signal
   *   -> this->on_RequestProductionInterface_slot
   * - this->performServerConnecting_signal
   *   -> Manager->performServerConnecting
   * - this->performServerDisconnecting_signal
   *   -> Manager->performServerDisconnecting
   * - this->performServerEcho_signal
   *   -> Manager->performServerEcho
   * - this->performServerAuthorization_signal
   *   -> Manager->performServerAuthorization
   * - this->performTransponderFirmwareLoading_signal
   *   -> Manager->performTransponderFirmwareLoading
   * - this->performTransponderFirmwareReloading_signal
   *   -> Manager->performTransponderFirmwareReloading
   * - this->performLocalFirmwareLoading_signal
   *   -> Manager->performLocalFirmwareLoading
   * - this->performFirmwareReading_signal
   *   -> Manager->performFirmwareReading
   * - this->performFirmwareErasing_signal
   *   -> Manager->performFirmwareErasing
   * - this->performDataReading_signal
   *   -> Manager->performDataReading
   * - this->performDataLoading_signal -> Manager->performDataLoading
   * - this->performDeviceUnlock_signal -> Manager->performDeviceUnlock
   * - this->performDeviceLock_signal -> Manager->performDeviceLock
   * - this->performPrintingLastTransponderSticker_signal
   *   -> Manager->performPrintingLastTransponderSticker
   * - this->performPrintingCustomTransponderSticker_signal
   *   -> Manager->performPrintingCustomTransponderSticker
   * - this->performStickerPrinterCommandScript_signal
   *   -> Manager->performStickerPrinterCommandScript
   * - this->applySettings_signal -> Manager->applySettings
   *
   *   Also, create a thread for ClientManager
   */
  void createManagerInstance(void);
  /*!
   * Create LogSystem and perform connections:
   * - this->applySettings_signal -> Logger->applySettings
   * - this->loggerClear_signal -> Logger->clear
   *
   * and move it in its own thread.
   */
  void createLoggerInstance(void);
  /*!
   * Create InteractionSystem instance and perform connections:
   * - Interactor->logging -> Logger->generate
   * - this->applySettings_signal -> Interactor->applySettings
   */
  void createInteractorInstance(void);

 private slots:
  /*!
   * Production interface was requested...
   */
  void on_RequestProductionInterface_slot(void);

 signals:
  /*!
   * Need to reload settings
   */
  void applySettings_signal(void);

  // Сигналы для логгера
  /*!
   * Need to clear logger outputs...
   */
  void loggerClear_signal(void);

  // Сигналы для менеджера
  /*!
   * Order manager to connect to the server
   */
  void performServerConnecting_signal(void);
  /*!
   * Order manager to disconnect from the server
   */
  void performServerDisconnecting_signal(void);
  /*!
   * Order manager to send echo request to the manager
   */
  void performServerEcho_signal(void);
  /*!
   * Order manager to authorize on the server
   * \param data server credentials
   */
  void performServerAuthorization_signal(
      const QSharedPointer<QMap<QString, QString>> data);
  /*!
   * Order manager to load firmware to the device
   * \param model transponder data
   */
  void performTransponderFirmwareLoading_signal(MapModel* model);
  /*!
   * Order manager to reload firmware to the device
   * \param model transponder data
   * \param pan device PAN
   */
  void performTransponderFirmwareReloading_signal(MapModel* model,
                                                  const QString& pan);
  /*!
   * Order manager to load local firmware to the device
   * \param path firmware file
   */
  void performLocalFirmwareLoading_signal(const QString& path);
  /*!
   * Order manager to read device firmware
   */
  void performFirmwareReading_signal(void);
  /*!
   * Order manage to erase device firmware
   */
  void performFirmwareErasing_signal(void);
  /*!
   * Order manager to read user data from device
   */
  void performDataReading_signal(void);
  /*!
   * Order manager to load user data to device
   * \param path user data image
   */
  void performDataLoading_signal(const QString& path);
  /*!
   * Order manager to unlock device memory
   */
  void performDeviceUnlock_signal(void);
  /*!
   * Order manager to lock device memory
   */
  void performDeviceLock_signal(void);
  /*!
   * Order manager to print last transponder sticker
   */
  void performPrintingLastTransponderSticker_signal(void);
  /*!
   * Order manager to print custom sticker
   * \param data custom sticker parameters
   */
  void performPrintingCustomTransponderSticker_signal(
      const QSharedPointer<QMap<QString, QString>> data);
  /*!
   * Order manager ot execute printer command script
   * \param commandScript commands to execute
   */
  void performStickerPrinterCommandScript_signal(
      const QSharedPointer<QStringList> commandScript);
};

#endif  // MAINWINDOW_KERNEL_H
