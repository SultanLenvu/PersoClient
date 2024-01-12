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
#include "GUI/testing_gui.h"
#include "General/definitions.h"
#include "General/hash_model.h"
#include "Log/log_system.h"

/*!
 * Main window, which gets created in main.cpp
 * Holds loggers, client manager, and user interaction system.
 */
class MainWindowKernel : public QMainWindow
{
  Q_OBJECT

 private:
  //! \cond
  QSize DesktopGeometry;
  AbstractGUI* CurrentGUI;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QMenu* HelpMenu;

  QAction* OpenMasterInterfaceAct;
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

  //! Transponder data model
  HashModel* TransponderDataModel;

 public:
  /*!
   * Default constructor:
   * - Initialize object name
   * - Set current gui mode to nullptr
   * - Initialize logger, interaction system, manager
   * - Initialize data models
   * - Initialize auth interface
   * - Register HashMap as meta type
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
  void on_RollbackProductionLinePushButton_slot(void);
  void on_PrintBoxStickerButton_slot(void);
  void on_PrintPalletStickerButton_slot(void);

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
  void on_AuthorizationInterfaceRequest_slot(void);
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
   * Create widgets and layout of testing interface
   */
  void createTestingInterface(void);
  /*!
   * Connect testing interface signals to slots
   */
  void connectTestingInterface(void);

  /*!
   * Initialize QActions for top menu
   */
  void createTopMenuActions(void);
  /*!
   * Create top menu depending on which interface is active
   * right now
   */
  void createTopMenu(void);

  /*!
   * Create ClientManager instance, do the signal-slot plumbing,
   * create dedicated thread and move ClientManager there
   */
  void createManagerInstance(void);
  /*!
   * Obtain LogSystem instance, do the signal-slot plumbing
   * and move it in its dedicated thread
   */
  void createLoggerInstance(void);
  /*!
   * Obtain InteractionSystem instance and do the signal-slot plumbing
   */
  void createInteractorInstance(void);

  /*!
   * Allocate and initialize transponder data model
   */
  void createModels(void);
  /*!
   * Register QSharedPointer<QHash<QString, QString>>> as meta type
   */
  void registerMetaTypes(void);

 private slots:
  /*!
   * Production interface was requested. Create production interface
   */
  void on_RequestProductionInterface_slot(void);
  /*!
   * Adjust window size
   */
  void on_VisibilityChanged_slot(void);
  /*!
   * Show new data from the model
   */
  void displayTransponderData_slot(
      QSharedPointer<QHash<QString, QString>> data);

 signals:
  /*!
   * Need to reload settings
   */
  void applySettings_signal(void);

  // Сигналы для логгера
  /*!
   * Need to clear logger outputs
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
      const QSharedPointer<QHash<QString, QString>> data);
  /*!
   * Order manager to load firmware to the device
   */
  void performTransponderFirmwareLoading_signal();
  /*!
   * Order manager to reload firmware to the device
   * \param pan device PAN
   */
  void performTransponderFirmwareReloading_signal(const QString& pan);
  /*!
   * Order manager to roll back production line
   */
  void rollbackProductionLine_signal(void);
  /*!
   * Order manager to print box sticker
   * \param[in] data Box sticker parameters
   */
  void performBoxStickerPrinting_signal(
      const QSharedPointer<QHash<QString, QString>> data);
  /*!
   * Order manager to print pallet sticker
   * \param[in] data Pallet sticker parameters
   */
  void performPalletStickerPrinting_signal(
      const QSharedPointer<QHash<QString, QString>> data);
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
  void performLastTransponderStickerPrinting_signal(void);
  /*!
   * Order manager to print custom sticker
   * \param data custom sticker parameters
   */
  void performCustomTransponderStickerPrinting_signal(
      const QSharedPointer<QHash<QString, QString>> data);
  /*!
   * Order manager ot execute printer command script
   * \param commandScript commands to execute
   */
  void performStickerPrinterCommandScript_signal(
      const QSharedPointer<QStringList> commandScript);
};

#endif  // MAINWINDOW_KERNEL_H
