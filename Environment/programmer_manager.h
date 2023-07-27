#ifndef PROGRAMMER_MANAGER_H
#define PROGRAMMER_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QThread>
#include <QVector>

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_exe_programmer.h"
#include "log_system.h"

class ProgrammerManager : public QObject {
  Q_OBJECT
public:
  enum PerfomingStatus { Completed, Failed };

private:
  LogSystem *Logger;
  QThread *ProgrammerThread;
  InterfaceProgrammer *Programmer;

  QFileInfo *FirmwareFileInfo;
  QFile *FirmwareFile;

  QFileInfo *UserDataFileInfo;
  QFile *UserDataFile;

  bool AutoLoadingFlag;
  bool ReadyIndicator;

  PerfomingStatus LastStatus;

public:
  explicit ProgrammerManager(QObject *parent);
  ~ProgrammerManager();

  InterfaceProgrammer *programmer(void) const;

  void performFirmwareLoading(const QString &path, bool unlockOption);
  void performFirmwareReading(void);
  void performFirmwareErasing(void);

  void performUserDataReading(void);
  void performUserDataLoading(const QString &path);

  void performDeviceUnlock(void);
  void performDeviceLock(void);

  void setFirmwareFile(const QString &path);
  void setLogger(LogSystem *logger);

private:
  void processingFirmwarePath(const QString &path);
  void processingUserDataPath(const QString &path);
  void buildProgrammerInstance(void);

private slots:
  void performingFinished(void);
  void performingCompleted(void);
  void performingFailed(void);

signals:
  void logging(const QString &log);
  void notifyUser(PerfomingStatus status);
};

#endif // PROGRAMMER_MANAGER_H
