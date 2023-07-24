#ifndef FIRMWARE_MANAGER_H
#define FIRMWARE_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QThread>

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_exe_programmer.h"
#include "log_system.h"

class FirmwareManager : public QObject {
  Q_OBJECT
public:
  enum PerfomingStatus { Completed, Failed };

private:
  LogSystem *Logger;
  QThread *ProgrammerThread;
  InterfaceProgrammer *Programmer;

  QFileInfo *FirmwareFileInfo;
  QFile *FirmwareFile;

  bool AutoLoadingFlag;
  bool ReadyIndicator;

  PerfomingStatus LastStatus;

public:
  explicit FirmwareManager(QObject *parent);
  ~FirmwareManager();

  InterfaceProgrammer *programmer(void) const;

  void performErasing(void);
  void performLoading(void);

  void setFirmwareFile(const QString &path);
  void setLogger(LogSystem *logger);

private:
  void processingFirmwarePath(const QString &path);
  void buildProgrammerInstance(void);

private slots:
  void performingFinished(void);
  void performingCompleted(void);
  void performingFailed(void);

signals:
  void logging(const QString &log);
  void notifyUser(PerfomingStatus status);
};

#endif // FIRMWARE_MANAGER_H
