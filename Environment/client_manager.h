#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <QApplication>
#include <QElapsedTimer>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "General/definitions.h"
#include "General/types.h"
#include "Programmers/interface_programmer.h"
#include "Programmers/jlink_exe_programmer.h"
#include "StickerPrinter/isticker_printer.h"
#include "StickerPrinter/te310_printer.h"
#include "log_system.h"
#include "perso_client.h"
#include "transponder_seed_model.h"

class ClientManager : public QObject {
  Q_OBJECT
 public:
  enum OperationState { Ready, WaitingExecution, Failed, Completed };

 private:
  QString CurrentLogin;
  QString CurrentPassword;

  OperationState CurrentState;
  QString NotificationText;

  QThread* ClientThread;
  PersoClient* Client;
  QThread* ProgrammerThread;
  IProgrammer* Programmer;

  QEventLoop* WaitingLoop;
  QTimer* ODQTimer;
  QTimer* ODTimer;
  QElapsedTimer* ODMeter;

  IStickerPrinter* Printer;

 public:
  explicit ClientManager(QObject* parent);
  ~ClientManager();

  IProgrammer* programmer(void) const;

  void performServerConnecting(void);
  void performServerDisconnecting(void);
  void performServerEcho(void);
  void performServerAuthorization(const QMap<QString, QString>* data);
  void performTransponderFirmwareLoading(TransponderSeedModel* model);
  void performTransponderFirmwareReloading(TransponderSeedModel* model,
                                           const QString& pan);

  void performLocalFirmwareLoading(const QString& path, bool unlockOption);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);

  void performDataReading(void);
  void performDataLoading(const QString& path);

  void performDeviceUnlock(void);
  void performDeviceLock(void);

  void applySettings(void);

 private:
  void loadSettings(void);
  void createProgrammerInstance(void);
  void createClientInstance(void);
  void createPrinterInstance(void);
  void createWaitingLoop(void);
  void createTimers(void);
  void setupODQTimer(uint32_t msecs);

  bool startOperationExecution(const QString& operationName);
  void endOperationExecution(const QString& operationName);

  void deleteClientInstance(void);
  void deleteProgrammerInstance(void);

 private slots:
  void proxyLogging(const QString& log);

  void on_ProgrammerOperationFinished_slot(IProgrammer::ExecutionStatus status);
  void on_ClientOperationFinished_slot(PersoClient::ExecutionStatus status);
  void on_ODTimerTimeout_slot(void);
  void on_ODQTimerTimeout_slot(void);

 signals:
  void logging(const QString& log);
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);
  void operationPerfomingStarted(void);
  void operationStepPerfomed(void);
  void operationPerformingEnded(void);
  void waitingEnd(void);
  void createProductionInterface_signal(void);
  void applySettings_signal(void);

  // Сигналы для программатора
  void getUcid_signal(QString* ucid);
  void loadFirmware_signal(QFile* firmware);
  void loadFirmwareWithUnlock_signal(QFile* firmware);
  void readFirmware_signal(void);
  void eraseFirmware_signal(void);
  void readData_signal(void);
  void loadData_signal(QFile* data);
  void unlockDevice_signal(void);
  void lockDevice_signal(void);

  // Сигналы для клиента
  void connectToPersoServer_signal(void);
  void disconnectFromPersoServer_signal(void);

  void requestEcho_signal(void);
  void requestAuthorize_signal(const QMap<QString, QString>* requestParameters);
  void requestTransponderRelease_signal(
      const QMap<QString, QString>* requestParameters,
      QFile* firmware);
  void requestTransponderReleaseConfirm_signal(
      const QMap<QString, QString>* requestParameters,
      QMap<QString, QString>* responseParameters);
  void requestTransponderRerelease_signal(
      const QMap<QString, QString>* requestParameters,
      QFile* firmware);
  void requestTransponderRereleaseConfirm_signal(
      const QMap<QString, QString>* requestParameters,
      QMap<QString, QString>* responseParameters);
};

#endif  // CLIENT_MANAGER_H
