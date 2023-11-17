#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <QApplication>
#include <QElapsedTimer>
#include <QMutex>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "General/definitions.h"
#include "General/hash_model.h"
#include "General/types.h"
#include "Log/log_system.h"
#include "Programmers/interface_programmer.h"
#include "Programmers/jlink_exe_programmer.h"
#include "StickerPrinter/isticker_printer.h"
#include "StickerPrinter/te310_printer.h"
#include "perso_client.h"

/*!
 * \brief Client Manager
 *
 * Holds:
 * - programmer interface
 * - server credentials
 * - sticker printer interface
 * - event loop
 * - Timers
 * - Threads
 */
class ClientManager : public QObject {
  Q_OBJECT

 private:
  //! Is logging enabled
  bool LogEnable;

  //! PersoServer login
  QString CurrentLogin;
  //! PersoServer password
  QString CurrentPassword;

  //! Client for PersoServer
  PersoClient* Client;
  //! Strings for PersoClient statuses?
  QHash<PersoClient::ReturnStatus, QString> ClientReturnStatusMatch;

  //! Programmer interface
  IProgrammer* Programmer;
  //! Strings for IProgrammer statuses?
  QHash<IProgrammer::ReturnStatus, QString> ProgrammerReturnStatusMatch;

  //! Sticker printer interface
  IStickerPrinter* StickerPrinter;
  //! Strings for IStickerPrinter statuses?
  QHash<IStickerPrinter::ReturnStatus, QString> StickerPrinterReturnStatusMatch;

  //! Self-explanatory
  QMutex Mutex;

 public:
  /*! Default constructor
   * \param[in] parent QObject parent
   */
  explicit ClientManager(QObject* parent);
  //! Default destructor
  ~ClientManager();

 public slots:
  /*! No-op!
   * \todo why?
   */
  void on_InstanceThreadStarted_slot(void);

  // Сервер
  /*!
   * Connect to server
   */
  void performServerConnecting(void);
  /*!
   * Disconnect from server
   */
  void performServerDisconnecting(void);
  /*!
   * Send echo request to server
   */
  void performServerEcho(void);
  /*!
   * Authorize on the server using login and password from QMap
   * \param[in] data A map containing "login" and "password" keys
   */
  void performServerAuthorization(
      const QSharedPointer<QHash<QString, QString>> data);
  /*!
   * Load firmware to the device. The following steps are performed:
   * - Unlock device flash memory;
   * - Read device's UCID;
   * - Download firmware from PersoServer;
   * - Load the firmware to the device;
   * - Report firmware loading to the server;
   * - Build model;
   * - Print device sticker.
   */
  void performTransponderFirmwareLoading(void);
  /*!
   * Reloads device firmware. The following steps are performed:
   * - Unlock device memory;
   * - Read device's UCID;
   * - Request device re-release from PersoServer;
   * - Loads the firmware to the device;
   * - Request device re-release confirmation from PersoServer;
   * - Build model;
   * - Print device sticker.
   *
   * \param[in] pan PAN
   */
  void performTransponderFirmwareReloading(const QString& pan);
  /*!
   * Roll production line back
   */
  void rollbackProductionLine(void);
  /*!
   * Print box sticker
   * \param[in] param Parameters of the sticker
   */
  void performBoxStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> param);
  /*!
   * Print pallet sticker
   * \param[in] param Parameters of the sticker
   */
  void performPalletStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> param);

  // Программатор
  /*!
   * Load firmware from local file.
   * \param[in] path Path to firmware file
   */
  void performLocalFirmwareLoading(const QString& path);
  /*!
   * Read firmware from the device
   */
  void performFirmwareReading(void);
  /*!
   * Erase firmware from the device
   */
  void performFirmwareErasing(void);
  /*!
   * Read data from the device
   */
  void performDataReading(void);
  /*!
   * Load data from the file to device
   * \param[in] path Path to data file
   */
  void performDataLoading(const QString& path);
  /*!
   * Unlock device memory
   */
  void performDeviceUnlock(void);
  /*!
   * Lock device memory
   */
  void performDeviceLock(void);

  // Стикер принтер
  /*!
   * Print last transponder sticker
   */
  void performLastTransponderStickerPrinting(void);
  /*!
   * Print custom transponder sticker
   * \param[in] data Parameters.
   * \sa IStickerPrinter::printTransponderSticker()
   */
  void performCustomTransponderStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> data);
  /*!
   * Execute custom printer script (wat?)
   * \param[in] commandScript List of printer commands
   * \sa IStickerPrinter::exec()
   */
  void performStickerPrinterCommandScript(
      const QSharedPointer<QStringList> commandScript);

  /*!
   * Call private loadSettings() method, emit applySettings_signal
   * and ask printer to apply new coniguration.
   */
  void applySettings(void);

 private:
  //! \cond
  Q_DISABLE_COPY(ClientManager);
  //! \endcond
  /*!
   * Load settings from QSettigns, namely:
   * - log_system/global_enable: Whether logging is enabled
   */
  void loadSettings(void);
  /*!
   * If logging is enabled, emits logging signal with a new log line
   * \param log a new log line
   */
  void sendLog(const QString& log) const;
  /*!
   * Create JLink.exe programmer instance do signal-slot plumbing:
   * Programmer->logging -> LogSystem::instance()->generate
   *
   * and fill ProgrammerReturnStatusMatch
   */
  void createProgrammerInstance(void);
  /*!
   * PersoClient instance, do signal-slot plumbing:
   * - Client->logging -> LogSystem::instance()->generate
   *
   * and fill ClientReturnStatusMatch
   */
  void createClientInstance(void);
  /*!
   * Create TE310Printer instance, connect it to the logging system
   * and fill StickerPrinterReturnStatusMatch
   */
  void createStickerPrinterInstance(void);

  /*!
   * Changes current state from Ready to WaitingExecution
   * and starts timeout timers. Fails if current state is not Ready.
   * \param[in] operationName operation name to 1) Query timeout value
   * and 2) log beginning
   */
  void startOperationPerforming(const QString& operationName);
  /*!
   * emit operationPerformingFinished, report operation results
   * to user and change current state to Ready.
   */
  void finishOperationPerforming(const QString& operationName);

  /*!
   * Report PersoClient errors to user
   * \param[in] status status to report
   * \param[in] operationName what operation it was
   */
  void processClientError(PersoClient::ReturnStatus status,
      const QString& operationName);
  /*!
   * Report Programmer errors to user
   * \param[in] status status to report
   * \param[in] operationName what operation it was
   */
  void processProgrammerError(IProgrammer::ReturnStatus status,
      const QString& operationName);
  /*!
   * Report Programmer errors to user
   * \param[in] status status to report
   * \param[in] operationName what operation it was
   */
  void processStickerPrintersError(IStickerPrinter::ReturnStatus status,
      const QString& operationName);

 signals:
  /*!
   * This signal is emitted to send a log line to the logger
   * \param[in] log New log line
   */
  void logging(const QString& log) const;
  /*!
   * This signal is emitted to notify user about something
   * \param[in] log New log line
   */
  void notifyUser(const QString& log);
  /*!
   * This signal is emitted to notify user about error
   * \param[in] log New log line
   */
  void notifyUserAboutError(const QString& log);
  /*!
   * This signal is emitted to notify user about
   * started operation
   * \param[in] operationName name of the new operation
   */
  void operationPerformingStarted(const QString& operationName);
  /*!
   * This signal is emitted to notify user about
   * finished operation
   * \param[in] operationName name of the finished operation
   */
  void operationPerformingFinished(const QString& operationName);
  /*!
   * This signal is emitted to GUI to create the production interface
   */
  void requestProductionInterface_signal(void);

  /*!
   * Emitted to show transponder data
   * \param data Data to display
   */
  void displayTransponderData_signal(
      QSharedPointer<QHash<QString, QString>> data);
};

#endif  // CLIENT_MANAGER_H
