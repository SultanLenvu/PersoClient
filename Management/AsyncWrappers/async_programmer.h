#ifndef ASYNCPROGRAMMER_H
#define ASYNCPROGRAMMER_H

#include "i_programmer.h"
#include "progressable_async_wrapper.h"

class AsyncProgrammer : public ProgressableAsyncWrapper {
  Q_OBJECT

 private:
  std::shared_ptr<IProgrammer> Programmer;

 public:
  Q_INVOKABLE explicit AsyncProgrammer(const QString& name,
                                       std::shared_ptr<IProgrammer> programmer);
  ~AsyncProgrammer() = default;

 public:
  void checkConfig(void);

  void programMemory(const QByteArray& data);
  void programMemoryWithUnlock(const QByteArray& data);
  void readMemory(void);
  void eraseMemory(void);

  void programUserData(const QByteArray& data);
  void readUserData(void);

  void readTransponderUcid(void);

  void unlockMemory(void);
  void lockMemory(void);

 private:
  Q_DISABLE_COPY_MOVE(AsyncProgrammer)

 signals:
  void transponderUcidReady(QString& ucid);
  void transponderFirmwareReady(QByteArray& firmware);
  void transponderUserDataReady(QByteArray& data);
};

#endif  // ASYNCPROGRAMMER_H
