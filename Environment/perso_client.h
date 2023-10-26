#ifndef PERSOCLIENT_H
#define PERSOCLIENT_H

#include <QByteArray>
#include <QDataStream>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>

#include "General/definitions.h"

/*!
 * \brief PersoServer client
 * Holds connection data and methods to access server services
 */
class PersoClient : public QObject {
 Q_OBJECT
 public:
  //! Command execution statuses
  enum ReturnStatus {
   Completed,
   FirmwareFileSavingError,
   RequestParameterError,
   ServerConnectionError,
   ServerNotResponding,
   ServerConnectionTerminated,
   ResponseParsingError,
   ResponseSyntaxError,
   AuthorizationNotExist,
   AuthorizationAccessDenied,
   AuthorizationNotActive,
   // Статусы возврата от сервера
   CommandSyntaxError,
   DatabaseError,
   TransponderNotFound,
   TransponderNotReleasedEarlier,
   AwaitingConfirmationError,
   IdenticalUcidError,
   ProductionLineMissed,
   ProductionLineNotActive,
   CurrentOrderRunOut,
   CurrentOrderAssembled,
   ProductionLineRollbackLimitError,
   BoxStickerPrintError,
   PalletStickerPrintError,
   NextTransponderNotFound,
   StartBoxAssemblingError,
   StartPalletAssemblingError,
  };
  //! \cond
  Q_ENUM(ReturnStatus);
  //! \endcond

  //! PersoClient statuses
  enum InstanceState {
   Ready,
   CreatingRequest,
   WaitingServerConnection,
   WaitingResponse,
   ProcessingResponse,
  };
  //! \cond
  Q_ENUM(InstanceState);
  //! \endcond

 private:
  //! Is logging enabled?
  bool LogEnable;
  //! Is server responses tracing enabled?
  bool ExtendedLoggingEnable;
  //! Perso Server IP
  QHostAddress PersoServerAddress;
  //! Perso Server port
  uint32_t PersoServerPort;

  //! Current Instance state
  InstanceState CurrentState;

  //! Server connection socket
  QTcpSocket* Socket;

  //! How many bytes were received from the server
  uint32_t ReceivedDataBlockSize;
  //! RX buffer
  QByteArray ReceivedDataBlock;
  //! TX buffer
  QByteArray TransmittedDataBlock;

  //! Server response command -> handler
  QHash<QString, ReturnStatus (PersoClient::*)(void)> ResponseHandlers;
  //! Templates for server response validation
  QHash<QString, QSharedPointer<QVector<QString>>> ResponseTemplates;
  //! Current command for the server, in the JSON format
  QJsonObject CurrentCommand;
  //! Last response from the server
  QJsonObject CurrentResponse;

  //! Unserialized server reponse
  QHash<QString, QString>* ResponseData;
  //! File to write firmware to
  QFile* Firmware;

  //! Timer for tracking timeouts
  QTimer* WaitTimer;
  //! Event loop for tracking timeouts
  QEventLoop* WaitingLoop;

  //! number as string -> status code
  QHash<QString, ReturnStatus> ServerStatusMatchTable;

 public:
  /*!
   * Construct object, create socket and timers, load settings
   * \param[in] parent QObject parent
   */
  explicit PersoClient(QObject* parent);
  /*!
   * Default destructor. Closes current socket connection
   * if it is opened.
   */
  ~PersoClient();

  /*!
   * Connect to server.
   * \return Completed if connection succeeded,
   * ServerConnectionError otherwise
   */
  ReturnStatus connectToServer(void);
  /*!
   * Disconnect from server
   * \return Completed
   */
  ReturnStatus disconnectFromServer(void);

  /*!
   * Request echo from the server
   * \return what transmitBlock returns
   */
  ReturnStatus requestEcho(void);
  /*!
   * Request authorization on the server
   * \param[in] requestAttributes map which should contain
   * "login" and "password" keys
   * \return what transmitBlock returns
   */
  ReturnStatus requestAuthorize(const QHash<QString, QString>* requestData);

  /*!
   * Request transponder release
   * \param[in] requestAttributes Map which should contain
   * "login" and "password" fields
   * \param[in] firmware file to save device firmware to
   * \return what transmitBlock returns
   */
  ReturnStatus requestTransponderRelease(
    const QHash<QString, QString>* requestData,
    QFile* firmware,
    QHash<QString, QString>* responseData);
  /*!
   * Request transponder release confirmation
   * \param[in] requestAttributes Map which should contain
   * "login" and "password" fields
   * \param[out] responseAttributes map in which some keys
   * will be copied. See processTransponderRelease for more
   * information
   * \return one of the following codes:
   * - RequestParameterError if requestAttributes or firmware is NULL
   * - ServerConnectionError if server connection has been failed
   * - ServerNotResponding if transmission timed out
   * - ServerError if server reported an error
   * - ResponseSyntaxError if server response is malformed
   * - Completed if command was completed successfully
   * 
   * In the last 3 cases, ProcessingStatus will contain the same value
   */
  ReturnStatus requestTransponderReleaseConfirm(
    const QHash<QString, QString>* requestData);
  /*!
   * Request transponder re-release
   * \param[in] requestAttributes Map which should contain
   * "login" and "password" fields
   * \param[in] firmware file to store transponder firmware
   * \return one of the following codes:
   * - RequestParameterError if requestAttributes or firmware is NULL
   * - ServerConnectionError if server connection has been failed
   * - ServerNotResponding if transmission timed out
   * - ServerError if server reported an error
   * - ResponseSyntaxError if server response is malformed
   * - ResponseSyntaxError if firmware saving failed
   * - Completed if command was completed successfully
   *
   * In last 4 cases, ProcessingStatus will contain the same value
   */
  ReturnStatus requestTransponderRerelease(
    const QHash<QString, QString>* requestData,
    QFile* firmware,
    QHash<QString, QString>* responseData);
  /*!
   * Request transponder re-release confirmation
   * \param[in] requestData Map which should contain
   * "login" and "password" fields
   * \param[out] responseAttributes map in which some keys
   * will be copied. See processTransponderRerelease for more
   * information
   * \return one of the following codes:
   * - RequestParameterError if requestAttributes or firmware is NULL
   * - ServerConnectionError if server connection has been failed
   * - ServerNotResponding if transmission timed out
   * - ServerError if server reported an error
   * - ResponseSyntaxError if server response is malformed
   * - Completed if command was completed successfully
   * 
   * In the last 3 cases, ProcessingStatus will contain the same value
   */
  ReturnStatus requestTransponderRereleaseConfirm(
    const QHash<QString, QString>* requestData);
  /*!
   * Request production line rollback
   * \param[in] requestData Map which must contain "login" and "password"
   * keys
   * \return return code of transmitDataBlock
   */
  ReturnStatus requestProductionLineRollback(
    const QHash<QString, QString>* requestData);

  /*!
   * Request box sticker printing
   * \param[in] requestData Map which must contain "pan" key
   * \return return code of transmitDataBlock
   */
  ReturnStatus requestBoxStickerPrint(
    const QHash<QString, QString>* requestData);
  /*!
   * Request box sticker re-printing
   * \return return code of transmitDataBlock
   */
  ReturnStatus requestBoxStickerReprint();
  /*!
   * Request pallet sticker printing
   * \param[in] requestData Map which must contain "pan" key
   * \return return code of transmitDataBlock
   */
  ReturnStatus requestPalletStickerPrint(
    const QHash<QString, QString>* requestData);
  /*!
   * Request pallet sticker re-printing
   * \return return code of transmitDataBlock
   */
  ReturnStatus requestPalletStickerReprint(void);

  /*!
   * Load settings using loadSettings function
   */
  void applySettings(void);

 private:
  //! \cond
  Q_DISABLE_COPY(PersoClient);
  //! \endcond
  /*!
   * Load settings from QSettings, namely:
   * - "log_system/global_enable": is logging enabled
   * - "log_system/extended_enable": is transmitted data tracing enabled
   * - "perso_client/server_ip": PersoServer IP
   * - "perso_client/server_port": PersoServer port
   */
  void loadSettings(void);
  /*!
   * emits logging signal if logging is enabled
   * \param[in] log New log line
   */
  void sendLog(const QString& log);

  /*!
   * Start timeout timer and try connecting to the server
   * \return true if connection was established, false otherwise
   * (e.g. timeouts)
   */
  bool processingServerConnection(void);
  /*!
   * Parse received data to JSON.
   */
  bool processingDataBlock(void);
  /*!
   * Serialize data for transmission to the server
   */
  void createTransmittedDataBlock(void);
  /*!
   * Send serialized data to the server
   */
  ReturnStatus transmitDataBlock(void);

  /*!
   * Compose echo command and store it in CurrentCommand
   */
  void createEcho(void);
  /*!
   * Compose authorization request and store it in CurrentCommand
   * \param[in] requestData map which must contain
   * "login" and "password" keys
   */
  void createAuthorization(const QHash<QString, QString>* requestData);
  /*!
   * Compose transponder release request and store it in CurrentCommand
   * \param[in] requestData map which must contain
   * "login" and "password" keys
   */
  void createTransponderRelease(const QHash<QString, QString>* requestData);
  /*!
   * Compose transponder release confirmation request
   * and store it in CurrentCommand
   * \param[in] requestData Map which must
   * contain "login", "password" and "ucid" keys
   */
  void createTransponderReleaseConfirm(
    const QHash<QString, QString>* requestData);
  /*!
   * Compose transponder re-release request and
   * store it in CurrentCommand
   * \param[in] requestData Map which must contain
   * "login", "password" and "pan" keys
   */
  void createTransponderRerelease(const QHash<QString, QString>* requestData);
  /*!
   * Compose transponder re-release confirmation request and
   * store it in CurrentCommand
   * \param[in] requestData Map which must contain
   * "login", "password", "pan" and "ucid" keys
   */
  void createTransponderRereleaseConfirm(
    const QHash<QString, QString>* requestData);
  /*!
   * Compose production line rollback request and store it in
   * CurrentCommand
   * \param[in] requestData Map which must contain
   * "login" and "password" keys
   */
  void createProductionLineRollback(const QHash<QString, QString>* requestData);
  /*!
   * Compose box sticker printing request and store it in CurrentCommand
   * \param[in] requestData Map which must contain "pan" key
   */
  void createBoxStickerPrint(const QHash<QString, QString>* requestData);
  /*!
   * Compose box sticker re-printing request and store it in
   * CurrentCommand
   */
  void createBoxStickerReprint(void);
  /*!
   * Compose pallet sticker printing request and store it in
   * CurrentCommand
   * \param[in] requestData Map which must contain "pan" key
   */
  void createPalletStickerPrint(const QHash<QString, QString>* requestData);
  /*!
   * Compose box sticker reprint request and store it in CurrentCommand
   */
  void createPalletStickerReprint(void);

  /*!
   * Process echo server response. Basically no-op
   * \return Completed
   */
  ReturnStatus processEcho(void);
  /*!
   * Process server authorization response. Basically no-op
   * \return Completed
   */
  ReturnStatus processAuthorization(void);
  /*!
   * Process server transponder release response.
   * Write received firmware to a file.
   * If saving succeeded, store "sn", "pan", "box_id", "pallet_id",
   * "order_id", "issuer_name", "transponder_model" in CurrentResponse
   * \return FirmwareFileSavingError if it is imposible to save
   * firmware, Completed otherwise
   */
  ReturnStatus processTransponderRelease(void);
  /*!
   * Process server transponder release confirmation response.
   * Basically no-op.
   * \return Completed
   */
  ReturnStatus processTransponderReleaseConfirm(void);
  /*!
   * Process server transponder re-release response.
   * Write received firmware to a file.
   * If saving succeeded, store "sn", "pan", "box_id", "pallet_id",
   * "order_id", "issuer_name", "transponder_model" in CurrentResponse
   * \return FirmwareFileSavingError if it is imposible to save
   * firmware, Completed otherwise
   */
  ReturnStatus processTransponderRerelease(void);
  /*!
   * Process server transponder re-release confirmation response.
   * Basically no-op.
   * \return Completed
   */
  ReturnStatus processTransponderRereleaseConfirm(void);

  /*!
   * Process server production line rollback response.
   * Basically no-op.
   * \return Completed
   */
  ReturnStatus processProductionLineRollback(void);

  /*!
   * Process server box sticker printing request. Basically no-op.
   * \return Completed
   */
  ReturnStatus processPrintBoxSticker(void);
  /*!
   * Process server last box sticker printing request. Basically no-op.
   * \return comleted
   */
  ReturnStatus processPrintLastBoxSticker(void);
  /*!
   * Process server pallet sticker printing request. Basically no-op.
   * \return Completed
   */
  ReturnStatus processPrintPalletSticker(void);
  /*!
   * Process server last pallet sticker printing request.
   * Basically no-op. \return comleted
   */
  ReturnStatus processPrintLastPalletSticker(void);

  /*!
   * Initialize timers and event loops: WaitTimer and WaitingLoop
   * to track timeouts. Perform signal-slot plumbing
   */
  void createTimers(void);
  /*!
   * Initialize Socket, and perform signal-slot plumbing:
   * - Socket->connected -> this->on_SocketConnected_slot
   * - Socket->disconnected -> this->on_SocketDisconneted_slot
   * - Socket->readyRead -> on_SocketReadyRead_slot
   */
  void createSocket(void);
  /*!
   * Initialize mapping server response command - handler
   */
  void createResponseHandlers(void);
  /*!
   * Create templates for response validation
   */
  void createResponseTemplates(void);
  /*!
   * Initialize textual status number - actual status mapping
   */
  void createServerStatusMatchTable(void);

 private slots:
  /*!
   * Notify user about established connection
   */
  void on_SocketConnected_slot(void);
  /*!
   * Notify user about closed connection
   */
  void on_SocketDisconnected_slot(void);

  /*!
   * Read received data from socket and deserialize them
   */
  void on_SocketReadyRead_slot(void);
  /*!
   * Abort the connection and report to user
   * \param socketError error to report
   */
  void on_SocketError_slot(QAbstractSocket::SocketError socketError);

  /*!
   * We hit a timeout. Abort the connection and report to user
   */
  void on_WaitTimerTimeout_slot(void);

 signals:
  /*!
   * Show a new log line
   * \param[in] log New log line
   */
  void logging(const QString& log);
  /*!
   * Connected to WaitingLoop's quit slot
   */
  void stopResponseWaiting(void);
};

#endif  // PERSOCLIENT_H
