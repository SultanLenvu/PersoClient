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
  explicit InterfaceProgrammer(QObject* parent);
  virtual ~InterfaceProgrammer();

public slots:
  virtual void connectDevice(void) = 0;
  virtual void loadFirmware(void) = 0;
  virtual void eraseFirmware(void) = 0;
  virtual void resetDevice(void) = 0;
  virtual void runDevice(void) = 0;
  virtual void exit(void) = 0;
  virtual void setLoadingFirmware(QFile *firmware) = 0;

signals:
  void logging(const QString &log);
  void operationFinished(void);
  void operationCompleted(void);
  void operationFailed(void);
};

#endif // ABSTRACTPROGRAMMER_H
