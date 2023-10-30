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

class ClientManager : public QObject {
  Q_OBJECT

 private:
  bool LogEnable;

  QString CurrentLogin;
  QString CurrentPassword;

  PersoClient* Client;
  QHash<PersoClient::ReturnStatus, QString> ClientReturnStatusMatch;

  IProgrammer* Programmer;
  QHash<IProgrammer::ReturnStatus, QString> ProgrammerReturnStatusMatch;

  IStickerPrinter* StickerPrinter;
  QHash<IStickerPrinter::ReturnStatus, QString> StickerPrinterReturnStatusMatch;

  QMutex Mutex;

 public:
  explicit ClientManager(QObject* parent);
  ~ClientManager();

 public slots:
  void on_InsctanceThreadStarted_slot(void);

  // Сервер
  void performServerConnecting(void);
  void performServerDisconnecting(void);
  void performServerEcho(void);
  void performServerAuthorization(
      const QSharedPointer<QHash<QString, QString>> data);
  void performTransponderFirmwareLoading(void);
  void performTransponderFirmwareReloading(const QString& pan);
  void rollbackProductionLine(void);
  void performBoxStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> param);
  void performPalletStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> param);

  // Программатор
  void performLocalFirmwareLoading(const QString& path);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);
  void performDataReading(void);
  void performDataLoading(const QString& path);
  void performDeviceUnlock(void);
  void performDeviceLock(void);

  // Стикер принтер
  void performLastTransponderStickerPrinting(void);
  void performCustomTransponderStickerPrinting(
      const QSharedPointer<QHash<QString, QString>> data);
  void performStickerPrinterCommandScript(
      const QSharedPointer<QStringList> commandScript);

  void applySettings(void);

 private:
  Q_DISABLE_COPY(ClientManager);
  void loadSettings(void);
  void sendLog(const QString& log) const;
  void createProgrammerInstance(void);
  void createClientInstance(void);
  void createStickerPrinterInstance(void);

  void startOperationPerforming(const QString& operationName);
  void finishOperationPerforming(const QString& operationName);

  void processClientError(PersoClient::ReturnStatus status,
                          const QString& operationName);
  void processProgrammerError(IProgrammer::ReturnStatus status,
                              const QString& operationName);
  void processStickerPrintersError(IStickerPrinter::ReturnStatus status,
                                   const QString& operationName);

 signals:
  void logging(const QString& log) const;
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);
  void operationPerfomingStarted(const QString& operationName);
  void operationPerformingFinished(const QString& operationName);
  void requestProductionInterface_signal(void);

  void displayTransponderData_signal(
      QSharedPointer<QHash<QString, QString>> data);
};

#endif  // CLIENT_MANAGER_H
