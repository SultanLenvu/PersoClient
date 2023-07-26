#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QString>

#include "../Environment/definitions.h"

class InterfaceProgrammer : public QObject
{
  Q_OBJECT
public:
  enum ProgrammerType { JLink };

protected:
  QFile *LoadingFirmware;
  QFile *LoadingUserData;

  ProgrammerType Type;

public:
  explicit InterfaceProgrammer(QObject *parent, ProgrammerType type);
  virtual ~InterfaceProgrammer();

  ProgrammerType type() const;

public slots:
  virtual void connectDevice(void) = 0;
  virtual void loadFirmware(void) = 0;
  virtual void loadUserData(void) = 0;
  virtual void readFirmware(void) = 0;
  virtual void readUserData(void) = 0;
  virtual void eraseFirmware(void) = 0;

  virtual void resetDevice(void) = 0;
  virtual void runDevice(void) = 0;
  virtual void unlockDevice(void) = 0;
  virtual void exit(void) = 0;

  void setLoadingFirmware(QFile *firmware);
  void setLoadingUserData(QFile *userData);

signals:
  void logging(const QString &log);
  void operationFinished(void);
  void operationCompleted(void);
  void operationFailed(void);
};

#endif // ABSTRACTPROGRAMMER_H
