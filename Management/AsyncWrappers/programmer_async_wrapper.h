#ifndef PROGRAMMERASYNCWRAPPER_H
#define PROGRAMMERASYNCWRAPPER_H

#include "i_programmer.h"
#include "progressable_async_wrapper.h"

class ProgrammerAsyncWrapper : public ProgressableAsyncWrapper {
 private:
  std::shared_ptr<IProgrammer> Programmer;

 public:
  Q_INVOKABLE explicit ProgrammerAsyncWrapper(
      const QString& name,
      std::shared_ptr<IProgrammer> programmer);
  ~ProgrammerAsyncWrapper() = default;

 public:
  void checkConfig(void);

  void programMemory(const TransponderFirmware& firmware);
  void programMemoryWithUnlock(const TransponderFirmware& firmware);
  void readMemory(void);
  void eraseMemory(void);

  void programUserData(const TransponderUserData& data);
  void readUserData(void);

  void readTransponderUcid(void);

  void unlockMemory(void);
  void lockMemory(void);

 private:
  Q_DISABLE_COPY_MOVE(ProgrammerAsyncWrapper)

 signals:
  void transponderUcidReady(QString& ucid);
  void transponderFirmwareReady(TransponderFirmware& firmware);
  void transponderUserDataReady(TransponderUserData& data);
};

#endif  // PROGRAMMERASYNCWRAPPER_H
