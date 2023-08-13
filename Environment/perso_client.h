#ifndef PERSOCLIENT_H
#define PERSOCLIENT_H

#include <QByteArray>
#include <QFile>
#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>

#include "user_settings.h"

class PersoClient : public QObject {
  Q_OBJECT

 private:
  QFile* Firmware;

  QTcpSocket* Socket;
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  QByteArray ReceivedData;

 public:
  explicit PersoClient(QObject* parent);
  ~PersoClient();

  QFile* getFirmware(void);
  void applySettings(UserSettings* settings);

  void connectToPersoServer(void);
  void disconnectFromPersoServer(void);
  void sendEchoRequest(void);

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
