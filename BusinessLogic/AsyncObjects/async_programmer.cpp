#include "async_programmer.h"

AsyncProgrammer::AsyncProgrammer(const QString& name,
                                 std::shared_ptr<IProgrammer> programmer)
    : ProgressableAsyncWrapper(name), Programmer(programmer) {}

void AsyncProgrammer::programMemory(const QString& fileName) {
  initOperation("programMemory");

  ReturnStatus ret = Programmer->programMemory(fileName);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemory", ret);
    return;
  }

  completeOperation("programMemory");
}

void AsyncProgrammer::programMemoryWithUnlock(const QString& fileName) {
  initOperation("programMemoryWithUnlock");

  ReturnStatus ret = Programmer->programMemoryWithUnlock(fileName);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemoryWithUnlock", ret);
    return;
  }

  completeOperation("programMemoryWithUnlock");
}

void AsyncProgrammer::readMemory() {
  initOperation("readMemory");

  ReturnStatus ret = Programmer->readMemory("memory.bin");
  if (ret != ReturnStatus::NoError) {
    processOperationError("readMemory", ret);
    return;
  }

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

void AsyncProgrammer::programUserData(const QString& fileName) {
  initOperation("programUserData");

  ReturnStatus ret = Programmer->programUserData(fileName);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programUserData", ret);
    return;
  }

  completeOperation("programUserData");
}

void AsyncProgrammer::readUserData() {
  initOperation("readUserData");

  ReturnStatus ret = Programmer->readUserData("user_data.bin");
  if (ret != ReturnStatus::NoError) {
    processOperationError("readUserData", ret);
    return;
  }

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

  sendLog(QString("Операция %1 успешно выполнена.").arg("readTransponderUcid"));
  emit executionFinished("readTransponderUcid", ReturnStatus::NoError);
  emit transponderUcidReady(ucid);
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
  initOperation("lockMemory");

  ReturnStatus ret = Programmer->lockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("lockMemory", ret);
    return;
  }

  completeOperation("lockMemory");
}
