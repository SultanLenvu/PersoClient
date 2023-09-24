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
  enum ExecutionStatus {
    NotExecuted,
    RequestParameterError,
    ServerConnectionError,
    ServerNotResponding,
    ServerConnectionTerminated,
    AuthorizationError,
    ResponseProcessingError,
    UnknownError,
    CompletedSuccessfully
  };

 private:
  enum InstanceState {
    Ready,
    RequestCreated,
    WaitingConnectionWithServer,
    ConnectedToServer,
    WaitingResponse,
    ResponseReceived,
    DisconnectedFromServer
  };

 private:
  InstanceState CurrentState;
  ExecutionStatus ReturnStatus;

  QTcpSocket* Socket;
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  uint32_t ReceivedDataBlockSize;
  QByteArray ReceivedDataBlock;
  QByteArray TransmittedDataBlock;

  QJsonObject CurrentCommand;
  QJsonDocument CurrentCommandDocument;
  QJsonObject CurrentResponse;
  QJsonDocument CurrentResponseDocument;

  QTimer* WaitTimer;
  QEventLoop* WaitingLoop;

  QMutex Mutex;

 public:
  explicit PersoClient(QObject* parent);
  ~PersoClient();

  void applySettings(void);

 public slots:
  void connectToPersoServer(void);
  void disconnectFromPersoServer(void);

  void requestAuthorize(const QString& login, const QString& password);
  void requestEcho(void);
  void requestTransponderRelease(QFile* firmware);
  void requestTransponderReleaseConfirm(
      const QMap<QString, QString>* requestAttributes,
      QMap<QString, QString>* responseAttributes);
  void requestTransponderRerelease(QFile* firmware, const QString& pan);
  void requestTransponderRereleaseConfirm(
      const QMap<QString, QString>* requestAttributes,
      QMap<QString, QString>* responseAttributes);

 private:
  bool processingServerConnection(void);

  void processingDataBlock(void);
  void createDataBlock(void);
  void transmitDataBlock(void);

  void createAuthorizationRequest(const QString& login,
                                  const QString& password);
  void createEchoRequest();
  void createTransponderRelease(void);
  void createTransponderReleaseConfirm(void);
  void createTransponderRerelease(void);
  void createTransponderRereleaseConfirm(void);

  void processAuthorizationResponse(void);
  void processEchoResponse(void);
  void processTransponderReleaseResponse(QFile* firmware);
  void processTransponderReleaseConfirmResponse(
      QMap<QString, QString>* responseAttributes);
  void processTransponderRereleaseResponse(QFile* firmware);
  void processTransponderRereleaseConfirmResponse(
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
  void operationFinished(ExecutionStatus status);
};

#endif  // PERSOCLIENT_H
