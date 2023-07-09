#ifndef FIRMWARE_MANAGER_H
#define FIRMWARE_MANAGER_H

#include <QApplication>
#include <QObject>
#include <QThread>

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_manual_programmer.h"
#include "log_system.h"

class FirmwareManager : public QObject {
  Q_OBJECT

public:
  enum LoaderStatus { Free, Success, Failed };

private:
  LogSystem *Logger;
  QThread *ProgrammerThread;
  InterfaceProgrammer *Programmer;

  QFileInfo *FirmwareFileInfo;
  QFile *FirmwareFile;

  bool AutoLoadingFlag;
  bool ReadyIndicator;

public:
  explicit FirmwareManager(QObject *parent, LogSystem *logger);
  ~FirmwareManager();

  InterfaceProgrammer *programmer(void) const;

  void performErasing(void);
  void performLoading(void);
  void performAutoLoading(void);

  void setFirmwareFile(const QString &path);

private:
  void processingFirmwarePath(const QString &path);
  void buildProgrammerInstance(void);
  void setReadyIndicator(void);

signals:
  void logging(const QString &log);
  void notifyUser(QString &data);
};

#endif // FIRMWARE_MANAGER_H
