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
    RequestParameterError,
    DataFileError,
    ProgrammatorError,
    Completed
  };
  Q_ENUM(ReturnStatus);

 public:
  enum ProgrammerType { JLink };
  Q_ENUM(ProgrammerType);

 protected:
  ProgrammerType Type;

 public:
  explicit IProgrammer(QObject* parent, ProgrammerType type);
  virtual ~IProgrammer();

  ProgrammerType type() const;

 public slots:
  virtual void getUcid(QString* ucid) = 0;
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
  bool checkFirmwareFile(const QFile* firmware);
  bool checkDataFile(const QFile* data);

 private:
  Q_DISABLE_COPY(IProgrammer);

 signals:
  void logging(const QString& log);
  void operationFinished(ReturnStatus status);
};

#endif  // ABSTRACTPROGRAMMER_H
