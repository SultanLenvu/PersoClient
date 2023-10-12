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
#include "General/types.h"
#include "Log/log_system.h"
#include "Programmers/interface_programmer.h"
#include "Programmers/jlink_exe_programmer.h"
#include "StickerPrinter/isticker_printer.h"
#include "StickerPrinter/te310_printer.h"
#include "perso_client.h"
#include "transponder_seed_model.h"

class ClientManager : public QObject {
  Q_OBJECT

 private:
  QString CurrentLogin;
  QString CurrentPassword;

  PersoClient* Client;
  QMap<PersoClient::ReturnStatus, QString> ClientReturnStatusMatch;

  IProgrammer* Programmer;
  QMap<IProgrammer::ReturnStatus, QString> ProgrammerReturnStatusMatch;

  IStickerPrinter* StickerPrinter;
  QMap<IStickerPrinter::ReturnStatus, QString> StickerPrinterReturnStatusMatch;

  QMutex Mutex;

 public:
  explicit ClientManager(QObject* parent);
  ~ClientManager();
  void on_InsctanceThreadStarted_slot(void);

  // Сервер
  void performServerConnecting(void);
  void performServerDisconnecting(void);
  void performServerEcho(void);
  void performServerAuthorization(
      const QSharedPointer<QMap<QString, QString>> data);
  void performTransponderFirmwareLoading(TransponderInfoModel* model);
  void performTransponderFirmwareReloading(TransponderInfoModel* model,
                                           const QString& pan);

  // Программатор
  void performLocalFirmwareLoading(const QString& path);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);
  void performDataReading(void);
  void performDataLoading(const QString& path);
  void performDeviceUnlock(void);
  void performDeviceLock(void);

  // Стикер принтер
  void performPrintingLastTransponderSticker(void);
  void performPrintingCustomTransponderSticker(
      const QSharedPointer<QMap<QString, QString>> data);
  void performStickerPrinterCommandScript(
      const QSharedPointer<QStringList> commandScript);

  void applySettings(void);

 private:
  Q_DISABLE_COPY(ClientManager);
  void loadSettings(void);
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

 private slots:
  void proxyLogging(const QString& log);

 signals:
  void logging(const QString& log);
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);
  void operationPerfomingStarted(const QString& operationName);
  void operationPerformingFinished(const QString& operationName);
  void requestProductionInterface_signal(void);
};

#endif  // CLIENT_MANAGER_H
