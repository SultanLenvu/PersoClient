#ifndef ASYNCPROGRAMMER_H
#define ASYNCPROGRAMMER_H

#include "i_programmer.h"
#include "progressable_async_wrapper.h"

class AsyncProgrammer final : public ProgressableAsyncWrapper {
  Q_OBJECT

 private:
  std::shared_ptr<IProgrammer> Programmer;

 public:
  explicit AsyncProgrammer(const QString& name,
                           std::shared_ptr<IProgrammer> programmer);
  ~AsyncProgrammer() = default;
  Q_DISABLE_COPY_MOVE(AsyncProgrammer)

 public:
  void programMemory(const QString& fileName);
  void programMemoryWithUnlock(const QString& fileName);
  void readMemory(void);
  void eraseMemory(void);

  void programUserData(const QString& fileName);
  void readUserData(void);

  void readTransponderUcid(void);

  void unlockMemory(void);
  void lockMemory(void);

 signals:
  void transponderUcidReady(const QString& ucid);
  void transponderFirmwareReady(const QByteArray& firmware);
  void transponderUserDataReady(const QByteArray& data);
};

#endif  // ASYNCPROGRAMMER_H
