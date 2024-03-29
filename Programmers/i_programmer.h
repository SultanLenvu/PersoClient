#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "transponder_firmware.h"
#include "transponder_user_data.h"
#include "types.h"

class IProgrammer {
 public:
  explicit IProgrammer() = default;
  virtual ~IProgrammer() = default;

 public:
  virtual ReturnStatus checkConfig(void) = 0;

  virtual ReturnStatus programMemory(const TransponderFirmware& firmware) = 0;
  virtual ReturnStatus programMemoryWithUnlock(
      const TransponderFirmware& firmware) = 0;
  virtual ReturnStatus readMemory(TransponderFirmware& data) = 0;
  virtual ReturnStatus eraseMemory(void) = 0;

  virtual ReturnStatus programUserData(const TransponderUserData& data) = 0;
  virtual ReturnStatus readUserData(TransponderUserData& data) = 0;

  virtual ReturnStatus readTransponderUcid(QString& ucid) = 0;

  virtual ReturnStatus unlockMemory(void) = 0;
  virtual ReturnStatus lockMemory(void) = 0;

 private:
  Q_DISABLE_COPY_MOVE(IProgrammer)
};

#endif  // ABSTRACTPROGRAMMER_H
