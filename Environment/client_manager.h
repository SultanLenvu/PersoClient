#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QThread>
#include <QVector>

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_exe_programmer.h"
#include "log_system.h"
#include "perso_client.h"

class ClientManager : public QObject {
  Q_OBJECT
 public:
  enum PerfomingStatus { Completed, Failed };

 private:
  PersoClient* Client;
  QThread* ProgrammerThread;
  InterfaceProgrammer* Programmer;

  QFileInfo* FirmwareFileInfo;
  QFile* FirmwareFile;

  QFileInfo* UserDataFileInfo;
  QFile* UserDataFile;

  bool ReadyIndicator;
  PerfomingStatus LastStatus;

 public:
  explicit ClientManager(QObject* parent);
  ~ClientManager();

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

  void setFirmwareFile(const QString& path);

 private:
  void processingFirmwarePath(const QString& path);
  void processingUserDataPath(const QString& path);
  void buildProgrammerInstance(void);

 private slots:
  void proxyLogging(const QString& log);

  void performingFinished(void);
  void performingCompleted(void);
  void performingFailed(void);

 signals:
  void logging(const QString& log);
  void notifyUser(const QString& log);
  void notifyUserAboutError(const QString& log);
};

#endif  // CLIENT_MANAGER_H
