#include "async_programmer.h"

AsyncProgrammer::AsyncProgrammer(const QString& name,
                                 std::shared_ptr<IProgrammer> programmer)
    : ProgressableAsyncWrapper(name), Programmer(programmer) {}

void AsyncProgrammer::checkConfig() {}

void AsyncProgrammer::programMemory(const QByteArray& firmware) {
  initOperation("programMemory");

  ReturnStatus ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemory", ret);
    return;
  }

  completeOperation("programMemory");
}

void AsyncProgrammer::programMemoryWithUnlock(const QByteArray& firmware) {
  initOperation("programMemoryWithUnlock");

  ReturnStatus ret = Programmer->programMemoryWithUnlock(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemoryWithUnlock", ret);
    return;
  }

  completeOperation("programMemoryWithUnlock");
}

void AsyncProgrammer::readMemory() {
  initOperation("readMemory");

  QByteArray firmware;
  ReturnStatus ret = Programmer->readMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("readMemory", ret);
    return;
  }
  emit transponderFirmwareReady(firmware);

  completeOperation("readMemory");
}

void AsyncProgrammer::eraseMemory() {
  initOperation("eraseMemory");

  ReturnStatus ret = Programmer->eraseMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("eraseMemory", ret);
    return;
  }

  completeOperation("eraseMemory");
}

void AsyncProgrammer::programUserData(const QByteArray& data) {
  initOperation("programUserData");

  ReturnStatus ret = Programmer->programUserData(data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programUserData", ret);
    return;
  }

  completeOperation("programUserData");
}

void AsyncProgrammer::readUserData() {
  initOperation("readUserData");

  QByteArray userData;
  ReturnStatus ret = Programmer->readUserData(userData);
  if (ret != ReturnStatus::NoError) {
    processOperationError("readUserData", ret);
    return;
  }

  emit transponderUserDataReady(userData);
  completeOperation("readUserData");
}

void AsyncProgrammer::readTransponderUcid() {
  initOperation("readTransponderUcid");

  QString ucid;
  ReturnStatus ret = Programmer->readTransponderUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    processOperationError("readTransponderUcid", ret);
    return;
  }

  emit transponderUcidReady(ucid);
  completeOperation("readTransponderUcid");
}

void AsyncProgrammer::unlockMemory() {
  initOperation("unlockMemory");

  ReturnStatus ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("unlockMemory", ret);
    return;
  }

  completeOperation("unlockMemory");
}

void AsyncProgrammer::lockMemory() {
  initOperation("unlockMemory");

  ReturnStatus ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("unlockMemory", ret);
    return;
  }

  completeOperation("unlockMemory");
}
