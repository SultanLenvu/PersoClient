#ifndef PERSO_SERVER_CONNECTION_H
#define PERSO_SERVER_CONNECTION_H

#include <QCoreApplication>
#include <QDataStream>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>

#include "abstract_client_command.h"
#include "abstract_server_connection.h"

class PersoServerConnection : public AbstractServerConnection {
  Q_OBJECT
 public:
  enum CommandId {
    Echo,
    LogIn,
    LogOut,
    Update,

    Release,
    ConfirmRelease,
    Rerelease,
    ConfirmRerelease,
    Rollback,

    PrintBoxSticker,
    PrintLastBoxSticker,
    PrintPalletSticker,
    PrintLastPalletSticker,

  };
  Q_ENUM(CommandId)

 private:
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  std::unique_ptr<QTcpSocket> Socket;

  std::shared_ptr<AbstractClientCommand> CurrentCommand;
  std::unordered_map<CommandId, std::shared_ptr<AbstractClientCommand>>
      Commands;

  uint32_t ReceivedDataBlockSize;
  QByteArray ReceivedDataBlock;

  std::unique_ptr<QTimer> WaitTimer;
  std::unique_ptr<QEventLoop> WaitingLoop;

 public:
  explicit PersoServerConnection(const QString& name);
  ~PersoServerConnection();

  // AbstractServerConnection interface
 public:
  virtual ReturnStatus connect() override;
  virtual void disconnect() override;
  virtual ReturnStatus echo() override;

  virtual ReturnStatus logIn(const StringDictionary& param) override;
  virtual ReturnStatus logOut() override;

  virtual ReturnStatus update(StringDictionary& result) override;
  virtual ReturnStatus getTransponderData(const StringDictionary& param,
                                          StringDictionary& result) override;

  virtual ReturnStatus release(StringDictionary& result) override;
  virtual ReturnStatus confirmRelease(const StringDictionary& param) override;

  virtual ReturnStatus rerelease(const StringDictionary& param,
                                 StringDictionary& result) override;
  virtual ReturnStatus confirmRerelease(const StringDictionary& param) override;

  virtual ReturnStatus rollback(void) override;

  virtual ReturnStatus printBoxSticker(const StringDictionary& param) override;
  virtual ReturnStatus printLastBoxSticker(void) override;

  virtual ReturnStatus printPalletSticker(
      const StringDictionary& param) override;
  virtual ReturnStatus printLastPalletSticker(void) override;

  virtual void applySettings(void) override;

 private:
  Q_DISABLE_COPY_MOVE(PersoServerConnection)
  void loadSettings(void);
  void sendLog(const QString& log);

  ReturnStatus transmitDataBlock(const QByteArray& dataBlock);
  bool waitResponse(void);

  // Фабричные методы
  void createTimers(void);
  void createSocket(void);
  void createCommands(void);

 private slots:
  void socketConnected_slot(void);
  void socketDisconnected_slot(void);

  void socketReadyRead_slot(void);
  void socketError_slot(QAbstractSocket::SocketError socketError);

  void waitTimerTimeout_slot(void);

 signals:
  void stopResponseWaiting(void);
};

#endif  // PERSO_SERVER_CONNECTION_H
