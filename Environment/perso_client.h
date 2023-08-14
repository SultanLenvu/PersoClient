#ifndef PERSOCLIENT_H
#define PERSOCLIENT_H

#include <QByteArray>
#include <QFile>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSettings>
#include <QTcpSocket>

#include "Environment/definitions.h"

class PersoClient : public QObject {
  Q_OBJECT

 private:
  QFile* Firmware;

  QTcpSocket* Socket;
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  QJsonDocument* CurrentCommand;
  QJsonDocument* CurrentResponse;

 public:
  explicit PersoClient(QObject* parent);
  ~PersoClient();

  QFile* getFirmware(void);
  void applySettings(void);

  void connectToPersoServer(void);
  void disconnectFromPersoServer(void);

  void echoRequest(void);
  void getFirmwareRequest(void);

 private:
  void deleteFirmware(void);

 private slots:
  void on_SocketConnected_slot(void);
  void on_SocketDisconnected_slot(void);

  void on_SocketReadyRead_slot(void);
  void on_SocketError_slot(QAbstractSocket::SocketError socketError);

 signals:
  void logging(const QString& log);
};

#endif  // PERSOCLIENT_H
