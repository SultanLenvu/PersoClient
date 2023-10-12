#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "General/definitions.h"
#include "General/types.h"

class IProgrammer : public QObject {
  Q_OBJECT
 public:
  enum ReturnStatus {
    NotExecuted,
    FirmwareFileError,
    DataFileError,
    DriverMissing,
    ProgrammatorError,
    Completed
  };
  Q_ENUM(ReturnStatus);

  enum ProgrammerType { JLink };
  Q_ENUM(ProgrammerType);

 protected:
  ProgrammerType Type;

 public:
  explicit IProgrammer(QObject* parent, ProgrammerType type);
  virtual ~IProgrammer();

  ProgrammerType type() const;

 public:
  virtual ReturnStatus getUcid(QString* ucid) = 0;
  virtual ReturnStatus loadFirmware(QFile* firmware) = 0;
  virtual ReturnStatus loadFirmwareWithUnlock(QFile* firmware) = 0;
  virtual ReturnStatus readFirmware(void) = 0;
  virtual ReturnStatus eraseFirmware(void) = 0;

  virtual ReturnStatus loadData(QFile* data) = 0;
  virtual ReturnStatus readData(void) = 0;

  virtual ReturnStatus unlockDevice(void) = 0;
  virtual ReturnStatus lockDevice(void) = 0;

  virtual void applySettings() = 0;

 protected:
  bool checkFirmwareFile(const QFile* firmware);
  bool checkDataFile(const QFile* data);

 private:
  Q_DISABLE_COPY(IProgrammer);

 signals:
  void logging(const QString& log);
};

#endif  // ABSTRACTPROGRAMMER_H
