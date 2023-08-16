#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "../Environment/definitions.h"

class InterfaceProgrammer : public QObject {
  Q_OBJECT
 public:
  enum ProgrammerType { JLink };
  enum OperationStatus { Success, Failed };

 protected:
  ProgrammerType Type;

 public:
  explicit InterfaceProgrammer(QObject* parent, ProgrammerType type);
  virtual ~InterfaceProgrammer();

  ProgrammerType type() const;

 public slots:
  virtual void loadFirmware(const QString& firmwareFileName) = 0;
  virtual void loadFirmwareWithUnlock(const QString& firmwareFileName) = 0;
  virtual void readFirmware(void) = 0;
  virtual void eraseFirmware(void) = 0;

  virtual void loadUserData(const QString& userDataFileName) = 0;
  virtual void readUserData(void) = 0;

  virtual void unlockDevice(void) = 0;
  virtual void lockDevice(void) = 0;

  virtual void applySettings() = 0;

 protected:
  bool checkFileName(const QString& name);

 signals:
  void logging(const QString& log);
  void operationFinished(OperationStatus status);
};

#endif  // ABSTRACTPROGRAMMER_H
