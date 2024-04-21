#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "types.h"

class IProgrammer {
 public:
  explicit IProgrammer() = default;
  virtual ~IProgrammer() = default;
  Q_DISABLE_COPY_MOVE(IProgrammer)

 public:
  virtual ReturnStatus checkConfig(void) = 0;

  virtual ReturnStatus programMemory(const QByteArray& data) = 0;
  virtual ReturnStatus programMemory(const QString& data) = 0;
  virtual ReturnStatus programMemoryWithUnlock(const QByteArray& data) = 0;
  virtual ReturnStatus programMemoryWithUnlock(const QString& data) = 0;
  virtual ReturnStatus readMemory(QByteArray& data) = 0;
  virtual ReturnStatus readMemory(const QString& fileName) = 0;
  virtual ReturnStatus eraseMemory(void) = 0;

  virtual ReturnStatus programUserData(const QByteArray& data) = 0;
  virtual ReturnStatus programUserData(const QString& fileName) = 0;
  virtual ReturnStatus readUserData(QByteArray& data) = 0;
  virtual ReturnStatus readUserData(const QString& fileName) = 0;

  virtual ReturnStatus readTransponderUcid(QString& ucid) = 0;

  virtual ReturnStatus unlockMemory(void) = 0;
  virtual ReturnStatus lockMemory(void) = 0;
};

#endif  // ABSTRACTPROGRAMMER_H
