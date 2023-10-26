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
#include "General/types.h"

/*!
 * \brief PersoServer client
 * Holds connection data and methods to access server services
 */
class PersoClient : public QObject {
  Q_OBJECT
  public:
    //! Command execution statuses
    enum ReturnStatus {
      NotExecuted,
      RequestParameterError,
      ServerConnectionError,
      ServerNotResponding,
      ServerConnectionTerminated,
      AuthorizationNotExist,
      AuthorizationAccessDenied,
      AuthorizationNotActive,
      ResponseSyntaxError,
      ServerError,
      UnknownError,
      Completed
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
    //! Is tracing server responses enabled?
    bool ExtendedLoggingEnable;
    //! Perso Server IP
    QHostAddress PersoServerAddress;
    //! Perso Server port
    uint32_t PersoServerPort;

    //! Current Instance state
    InstanceState CurrentState;
    //! Current processing status
    ReturnStatus ProcessingStatus;

    //! Server connection socket
    QTcpSocket* Socket;

    //! How many bytes were received from the server
    uint32_t ReceivedDataBlockSize;
    //! RX buffer
    QByteArray ReceivedDataBlock;
    //! TX buffer
    QByteArray TransmittedDataBlock;

    //! Current command for the server, in the JSON format
    QJsonObject CurrentCommand;
    //! Last response from the server
    QJsonObject CurrentResponse;

    //! Timer to catch timeouts
    QTimer* WaitTimer;
    //! Event loop to catch timeouts
    QEventLoop* WaitingLoop;
    //! Have we got connection timeout?
    bool TimeoutIndicator;

  public:
    /*!
     * Construct object, create timers and socket, load settings
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
     * \return ServerConnectionError if connection succeeded,
     * Completed otherwise
     */
    ReturnStatus connectToServer(void);
    /*!
     * Disconnect from server
     * \return Completed
     */
    ReturnStatus disconnectFromServer(void);

    /*!
     * Request echo from the server
     * \return one of the following codes:
     * - ServerConnectionError if server connection has been failed,
     * - ServerNotResponding if transmission timed out,
     * - ServerError server reported an error
     * - ResponseSyntaxError if server response is malformed
     *
     * In the last 2 cases, ProcessingStatus will contain
     * the same value
     */
    ReturnStatus requestEcho(void);
    /*!
     * Request authorization on the server
     * \param[in] requestAttributes map which should contain
     * "login" and "password" keys
     * \return one of the following codes:
     * - RequestParameterError if requestAttributes is NULL
     * - ServerConnectionError if server connection has been failed
     * - ServerNotResponding if transmission timed out
     * - ServerError if server reported an error
     * - ResponseSyntaxError if server response is malformed
     * - AuthorizationNotExists if server reported non-existent
     *   authorization (???)
     * - AuthorizationNotActive if server reported inactive
     *   authorization (???)
     * - AuthorizationAccessDenied if server denied access
     * - Completed if server granted access
     *
     * In the last 6 cases, ProcessingStatus will contain
     * the same value 
     */
    ReturnStatus requestAuthorize(
        const QMap<QString, QString>* requestAttributes);

    /*!
     * Request transponder release
     * \param[in] requestAttributes Map which should contain
     * "login" and "password" fields
     * \param[in] firmware file to save device firmware to
     * \return one of the following codes:
     * - RequestParameterError if requestAttributes or firmware is NULL
     * - ServerConnectionError if server connection has been failed
     * - ServerNotResponding if transmission timed out
     * - ServerError if server reported an error
     * - ResponseSyntaxError if server response is malformed
     * - ServerError if saving firmware to a file failed
     * - Completed if command was completed successfully
     *
     * In the last 4 cases, ProcessingStatus will contain the same value
     */
    ReturnStatus requestTransponderRelease(
        const QMap<QString, QString>* requestAttributes,
        QFile* firmware);
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
        const QMap<QString, QString>* requestAttributes,
        QMap<QString, QString>* responseAttributes);
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
        const QMap<QString, QString>* requestAttributes,
        QFile* firmware);
    /*!
     * Request transponder re-release confirmation
     * \param[in] requestAttributes Map which should contain
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
        const QMap<QString, QString>* requestAttributes,
        QMap<QString, QString>* responseAttributes);

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
   * - "log_enable": is logging enabled
   * - "extended_enable": is server responce logging enabled
   * - "server_ip": PersoServer IP
   * - "server_port": PersoServer port
   */
  void loadSettings(void);
  /*!
   * emits logging signal if logging is enabled
   * \param[in] log New log line
   */
  void sendLog(const QString& log);
  /*!
   * Create WaitTimer and corresponding WaitingLoop to
   * track server timeouts
   */
  void createTimers(void);
  /*!
   * Initialise Socket
   */
  void createSocket(void);
  /*!
   * Start timeout timer and try connecting to the server
   * \return true if connection was established, false otherwise
   * (e.g. timeouts)
   */
  bool processingServerConnection(void);

  /*!
   * Parse received data to JSON.
   */
  void processingDataBlock(void);
  /*!
   * Serialize data for transmission to the server
   */
  void createTransmittedDataBlock(void);
  /*!
   * Send serialized data to the server
   */
  void transmitDataBlock(void);

  /*!
   * Compose echo command, store it in CurrentCommand and serialize it
   * using createTransmittedDataBlock
   */
  void createEcho(void);
  /*!
   * Compose authorization request, store it in CurrentCommand
   * and serialize it using createTransmittedDataBlock
   * \param[in] requestAttributes map which must contain
   * "login" and "password" keys
   */
  void createAuthorization(const QMap<QString, QString>* requestAttributes);
  /*!
   * Compose transponder release request, store it in CurrentCommand
   * and serialize it using createTransmittedDataBlock
   * \param[in] requestAttributes map which must contain
   * "login" and "password" keys
   */
  void createTransponderRelease(
      const QMap<QString, QString>* requestAttributes);
  /*!
   * Compose transponder release confirmation request,
   * store it in CurrentCommand and serialize it
   * using createTransmittedDataBlock
   * \param[in] requestAttributes map which must
   * contain "login", "password" and "ucid" keys
   */
  void createTransponderReleaseConfirm(
      const QMap<QString, QString>* requestAttributes);
  /*!
   * Compose transponder re-release request,
   * store it in CurrentCommand and serialize it
   * using createTransmittedDataBlock
   * \param[in] requestAttributes map which must contain
   * "login", "password" and "pan"
   * keys
   */
  void createTransponderRerelease(
      const QMap<QString, QString>* requestAttributes);
  /*!
   * Compose transponder re-release confirmation request,
   * store it in CurrentCommand and serialize it
   * using createTransmittedDataBlock
   * \param[in] requestAttributes map which must contain
   * "login", "password", "pan" and "ucid" keys
   */
  void createTransponderRereleaseConfirm(
      const QMap<QString, QString>* requestAttributes);

  /*!
   * Process echo server response. Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "data" or "response_name" fields, or "response_name"
   *   field is not "Echo")
   * - Completed if everything is OK.
   */
  void processEcho(void);
  /*!
   * Process server authorization response. Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "response_name" or "access" fields, or "response_name"
   *   field is not "Authorization")
   * - AuthorizationNotExists if "access" field is "NotExists"
   * - AuthorizationNotActive if "access" field is "NotActive"
   * - Completed if "access" field is "Allowed"
   * - AuthorizationAccessDenied otherwise
   */
  void processAuthorization(void);
  /*!
   * Process server transponder release response.
   * Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "response_name" or "firmware" fields, or "response_name"
   *   field is not "TransponderRelease")
   * - ServerError if it is impossible to open firmware file
   * - Completed otherwise
   * \param[in] firmware file to store firmware to
   */
  void processTransponderRelease(QFile* firmware);
  /*!
   * Process server transponder release confirmation response.
   * Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "response_name", "sn", "pan", "box_id", "pallet_id",
   *   "order_id" or "issuer_name" fields, or "response_name"
   *   field is not "TransponderReleaseConfirm")
   * - Completed otherwise
   * 
   * \param[in] responseAttributes map to store "sn", "pan",
   * "box_id", "pallet_id", "order_id" and "issuer_name"
   */
  void processTransponderReleaseConfirm(
      QMap<QString, QString>* responseAttributes);
  /*!
   * Process server transponder re-release response.
   * Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "response_name" or "firmware" fields, or "response_name"
   *   field is not "TransponderRerelease")
   * - ResponseSyntaxError if it is impossible to open firmware file
   * - Completed otherwise
   * \param[in] firmware file to store firmware to
   */
  void processTransponderRerelease(QFile* firmware);
  /*!
   * Process server transponder re-release confirmation response.
   * Set ProcessingStatus to:
   * - ServerError if server reported an error (return
   *   status is not NoError)
   * - ResponseSyntaxError, if answer is malformed
   *   (no "response_name", "sn", "pan", "box_id", "pallet_id",
   *   "order_id" or "issuer_name" fields, or "response_name"
   *   field is not "TransponderRereleaseConfirm")
   * - Completed otherwise
   * 
   * \param[in] responseAttributes map to store "sn", "pan",
   * "box_id", "pallet_id", "order_id" and "issuer_name"
   */
  void processTransponderRereleaseConfirm(
      QMap<QString, QString>* responseAttributes);

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
