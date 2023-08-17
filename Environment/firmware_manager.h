#ifndef FIRMWARE_MANAGER_H
#define FIRMWARE_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QVector>

#include "Programmers/interface_programmer.h"
#include "Programmers/jlink_exe_programmer.h"
#include "log_system.h"
#include "perso_client.h"

#include "General/definitions.h"
#include "General/types.h"

class FirmwareManager : public QObject {
  Q_OBJECT
 public:
  enum OperatingState {
    Ready,
    WaitingProgrammerProcessing,
    WaitingClientProcessing,
  };

 private:
  OperatingState State;

  QThread* ClientThread;
  PersoClient* Client;
  QThread* ProgrammerThread;
  InterfaceProgrammer* Programmer;

  QFile* FirmwareFile;
  QFile* DataFile;

  QMutex FirmwareMutex;
  QTimer* WaitTimer;
  QEventLoop* WaitingLoop;
  bool ReadyIndicator;

 public:
  explicit FirmwareManager(QObject* parent);
  ~FirmwareManager();

  InterfaceProgrammer* programmer(void) const;

  void performFirmwareLoading(const QString& path, bool unlockOption);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);

  void performDataReading(void);
  void performDataLoading(const QString& path);

  void performDeviceUnlock(void);
  void performDeviceLock(void);

  void performServerConnecting(void);
  void performServerDisconnecting(void);
  void performServerEchoRequest(void);
  void performServerFirmwareRequest(void);

  void performServerFirmwareLoading(void);

  void applySettings(void);

 private:
  void deleteFirmwareFile(void);
  void deleteDataFile(void);

  void createProgrammerInstance(void);
  void creareClientInstance(void);

 private slots:
  void proxyLogging(const QString& log);

  void on_ProgrammerOperationFinished_slot(OperationStatus status);
  void on_ClientOperationFinished_slot(OperationStatus status);

 signals:
  void logging(const QString& log);
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);

  // Сигналы для программатора
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
  void requestFirmware_signal(QFile* firmware);
};

#endif  // FIRMWARE_MANAGER_H
