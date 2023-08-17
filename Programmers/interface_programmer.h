#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "General/definitions.h"
#include "General/types.h"

class InterfaceProgrammer : public QObject {
  Q_OBJECT
 public:
  enum ProgrammerType { JLink };

 protected:
  ProgrammerType Type;

 public:
  explicit InterfaceProgrammer(QObject* parent, ProgrammerType type);
  virtual ~InterfaceProgrammer();

  ProgrammerType type() const;

 public slots:
  virtual void loadFirmware(QFile* firmware) = 0;
  virtual void loadFirmwareWithUnlock(QFile* firmware) = 0;
  virtual void readFirmware(void) = 0;
  virtual void eraseFirmware(void) = 0;

  virtual void loadData(QFile* data) = 0;
  virtual void readData(void) = 0;

  virtual void unlockDevice(void) = 0;
  virtual void lockDevice(void) = 0;

  virtual void applySettings() = 0;

 protected:
  bool checkFirmwareFile(QFile* firmware);
  bool checkDataFile(QFile* data);

 signals:
  void logging(const QString& log);
  void operationFinished(OperationStatus status);
};

#endif  // ABSTRACTPROGRAMMER_H
