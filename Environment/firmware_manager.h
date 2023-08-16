#ifndef FIRMWARE_MANAGER_H
#define FIRMWARE_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QVector>

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_exe_programmer.h"
#include "log_system.h"
#include "perso_client.h"

class FirmwareManager : public QObject {
  Q_OBJECT
 public:
  enum PerfomingStatus { Completed, Failed };

 private:
  QThread* ClientThread;
  PersoClient* Client;
  QThread* ProgrammerThread;
  InterfaceProgrammer* Programmer;

  QFileInfo* FirmwareFileInfo;
  QFile* FirmwareFile;

  QFileInfo* UserDataFileInfo;
  QFile* UserDataFile;

  QTimer* WaitTimer;
  QEventLoop WaitingLoop;
  bool ReadyIndicator;
  PerfomingStatus LastStatus;

 public:
  explicit FirmwareManager(QObject* parent);
  ~FirmwareManager();

  InterfaceProgrammer* programmer(void) const;

  void performFirmwareLoading(const QString& path, bool unlockOption);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);

  void performUserDataReading(void);
  void performUserDataLoading(const QString& path);

  void performDeviceUnlock(void);
  void performDeviceLock(void);

  void performServerConnecting(void);
  void performServerDisconnecting(void);
  void performServerEchoRequest(void);
  void performServerFirmwareRequest(void);

  void setFirmwareFile(const QString& path);
  void applySettings(void);

 private:
  void processingFirmwarePath(const QString& path);
  void processingUserDataPath(const QString& path);

  void createProgrammerInstance(void);
  void creareClientInstance(void);

 private slots:
  void proxyLogging(const QString& log);

  void on_ProgrammerOperationFinished_slot(PerfomingStatus status);

 signals:
  void logging(const QString& log);
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);

  // Сигналы для программатора
  void loadFirmware_signal(const QString& firmwareFileName);
  void loadFirmwareWithUnlock_signal(const QString& firmwareFileName);
  void readFirmware_signal(void);
  void eraseFirmware_signal(void);

  void readUserData_signal(void);
  void loadUserData_signal(const QString& userDataFileName);

  void unlockDevice_signal(void);
  void lockDevice_signal(void);
};

#endif  // FIRMWARE_MANAGER_H
