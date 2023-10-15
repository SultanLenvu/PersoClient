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

class PersoClient : public QObject {
  Q_OBJECT
 public:
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
  Q_ENUM(ReturnStatus);

  enum InstanceState {
    Ready,
    CreatingRequest,
    WaitingServerConnection,
    WaitingResponse,
    ProcessingResponse,
  };
  Q_ENUM(InstanceState);

 private:
  bool LogEnable;
  bool ExtendedLoggingEnable;
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  InstanceState CurrentState;
  ReturnStatus ProcessingStatus;

  QTcpSocket* Socket;

  uint32_t ReceivedDataBlockSize;
  QByteArray ReceivedDataBlock;
  QByteArray TransmittedDataBlock;

  QJsonObject CurrentCommand;
  QJsonObject CurrentResponse;

  QTimer* WaitTimer;
  QEventLoop* WaitingLoop;
  bool TimeoutIndicator;

 public:
  explicit PersoClient(QObject* parent);
  ~PersoClient();

  ReturnStatus connectToServer(void);
  ReturnStatus disconnectFromServer(void);

  ReturnStatus requestEcho(void);
  ReturnStatus requestAuthorize(
      const QMap<QString, QString>* requestAttributes);

  ReturnStatus requestTransponderRelease(
      const QMap<QString, QString>* requestAttributes,
      QFile* firmware);
  ReturnStatus requestTransponderReleaseConfirm(
      const QMap<QString, QString>* requestAttributes,
      QMap<QString, QString>* responseAttributes);
  ReturnStatus requestTransponderRerelease(
      const QMap<QString, QString>* requestAttributes,
      QFile* firmware);
  ReturnStatus requestTransponderRereleaseConfirm(
      const QMap<QString, QString>* requestAttributes,
      QMap<QString, QString>* responseAttributes);

  void applySettings(void);

 private:
  Q_DISABLE_COPY(PersoClient);
  void loadSettings(void);
  void sendLog(const QString& log);
  void createTimers(void);
  void createSocket(void);
  bool processingServerConnection(void);

  void processingDataBlock(void);
  void createTransmittedDataBlock(void);
  void transmitDataBlock(void);

  void createEcho(void);
  void createAuthorization(const QMap<QString, QString>* requestAttributes);
  void createTransponderRelease(
      const QMap<QString, QString>* requestAttributes);
  void createTransponderReleaseConfirm(
      const QMap<QString, QString>* requestAttributes);
  void createTransponderRerelease(
      const QMap<QString, QString>* requestAttributes);
  void createTransponderRereleaseConfirm(
      const QMap<QString, QString>* requestAttributes);

  void processEcho(void);
  void processAuthorization(void);
  void processTransponderRelease(QFile* firmware);
  void processTransponderReleaseConfirm(
      QMap<QString, QString>* responseAttributes);
  void processTransponderRerelease(QFile* firmware);
  void processTransponderRereleaseConfirm(
      QMap<QString, QString>* responseAttributes);

 private slots:
  void on_SocketConnected_slot(void);
  void on_SocketDisconnected_slot(void);

  void on_SocketReadyRead_slot(void);
  void on_SocketError_slot(QAbstractSocket::SocketError socketError);

  void on_WaitTimerTimeout_slot(void);

 signals:
  void logging(const QString& log);
  void stopResponseWaiting(void);
};

#endif  // PERSOCLIENT_H
