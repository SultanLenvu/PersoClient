#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "types.h"

class AbstractProgrammer : public QObject {
  Q_OBJECT
 public:
  enum ProgrammerType { JLinkExe };
  Q_ENUM(ProgrammerType);

 public:
  explicit AbstractProgrammer(const QString& name);
  virtual ~AbstractProgrammer();

 public:
  virtual ReturnStatus checkConfig(void) = 0;
  virtual ProgrammerType type() const = 0;
  virtual ReturnStatus programMemory(QFile& firmware) = 0;
  virtual ReturnStatus programMemoryWithUnlock(QFile& firmware) = 0;
  virtual ReturnStatus readMemory(void) = 0;
  virtual ReturnStatus eraseMemory(void) = 0;

  virtual ReturnStatus programUserData(QFile& data) = 0;
  virtual ReturnStatus readUserData(void) = 0;

  virtual ReturnStatus readUcid(QString& ucid) = 0;

  virtual ReturnStatus unlockMemory(void) = 0;
  virtual ReturnStatus lockMemory(void) = 0;

  virtual void applySettings() = 0;

 protected:
  bool checkFirmwareFile(const QFile& firmware);
  bool checkDataFile(const QFile& data);

 private:
  Q_DISABLE_COPY(AbstractProgrammer);

 signals:
  void logging(const QString& log);
};

#endif  // ABSTRACTPROGRAMMER_H
