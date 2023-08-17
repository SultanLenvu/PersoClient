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

  enum CommandExecutionStatus { NotExecuted, Completed, Failed };

 private:
  QFile* FirmwareFile;

  QTcpSocket* Socket;
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  uint32_t ReceivedDataBlockSize;
  QByteArray ReceivedDataBlock;
  QByteArray TransmittedDataBlock;

  QJsonDocument CurrentCommand;
  QJsonDocument CurrentResponse;
  CommandExecutionStatus CurrentCommandStatus;

  QTimer* WaitTimer;
  QEventLoop* WaitingLoop;

  QMutex Mutex;

 public:
  explicit PersoClient(QObject* parent);
  ~PersoClient();

  void applySettings(void);

  void connectToPersoServer(void);
  void disconnectFromPersoServer(void);

  void requestEcho(void);
  void requestFirmware(QFile* firmware);

 private:
  bool processingServerConnection(void);

  void processingDataBlock(void);
  void createDataBlock(void);
  void transmitDataBlock(void);

  void processingEchoResponse(QJsonObject* responseJson);
  void processingFirmwareResponse(QJsonObject* responseJson);

  bool checkFirmwareFile(QFile* firmware);
  void deleteFirmware(void);

 private slots:
  void on_SocketConnected_slot(void);
  void on_SocketDisconnected_slot(void);

  void on_SocketReadyRead_slot(void);
  void on_SocketError_slot(QAbstractSocket::SocketError socketError);

  void on_WaitTimerTimeout_slot(void);

 signals:
  void logging(const QString& log);
  void stopWaiting(void);
  void operationFinished(OperationStatus status);
};

#endif  // PERSOCLIENT_H
