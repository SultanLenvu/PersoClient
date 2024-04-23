#ifndef ASYNCPROGRAMMER_H
#define ASYNCPROGRAMMER_H

#include "i_programmer.h"
#include "loggable_object.h"
#include "progressable_async_object.h"

class AsyncProgrammer final : public AbstractAsyncObject,
                              public LoggableObject {
  Q_OBJECT

 private:
  std::shared_ptr<IProgrammer> Programmer;

 public:
  explicit AsyncProgrammer(std::shared_ptr<IProgrammer> programmer);
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
