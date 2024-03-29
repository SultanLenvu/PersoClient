#include "programmer_async_wrapper.h"
#include "jlink_exe_programmer.h"

ProgrammerAsyncWrapper::ProgrammerAsyncWrapper(
    const QString& name,
    std::shared_ptr<IProgrammer> programmer)
    : ProgressableAsyncWrapper(name), Programmer(programmer) {}

void ProgrammerAsyncWrapper::checkConfig() {}

void ProgrammerAsyncWrapper::programMemory(
    const TransponderFirmware& firmware) {
  initOperation("programMemory");

  ReturnStatus ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemory", ret);
    return;
  }

  completeOperation("programMemory");
}

void ProgrammerAsyncWrapper::programMemoryWithUnlock(
    const TransponderFirmware& firmware) {
  initOperation("programMemoryWithUnlock");

  ReturnStatus ret = Programmer->programMemoryWithUnlock(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programMemoryWithUnlock", ret);
    return;
  }

  completeOperation("programMemoryWithUnlock");
}

void ProgrammerAsyncWrapper::readMemory() {
  initOperation("readMemory");

  TransponderFirmware firmware;
  ReturnStatus ret = Programmer->readMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("readMemory", ret);
    return;
  }
  emit transponderFirmwareReady(firmware);

  completeOperation("readMemory");
}

void ProgrammerAsyncWrapper::eraseMemory() {
  initOperation("eraseMemory");

  ReturnStatus ret = Programmer->eraseMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("eraseMemory", ret);
    return;
  }

  completeOperation("eraseMemory");
}

void ProgrammerAsyncWrapper::programUserData(const TransponderUserData& data) {
  initOperation("programUserData");

  ReturnStatus ret = Programmer->programUserData(data);
  if (ret != ReturnStatus::NoError) {
    processOperationError("programUserData", ret);
    return;
  }

  completeOperation("programUserData");
}

void ProgrammerAsyncWrapper::readUserData() {
  initOperation("readUserData");

  TransponderUserData userData;
  ReturnStatus ret = Programmer->readUserData(userData);
  if (ret != ReturnStatus::NoError) {
    processOperationError("readUserData", ret);
    return;
  }

  emit transponderUserDataReady(userData);
  completeOperation("readUserData");
}

void ProgrammerAsyncWrapper::readTransponderUcid() {
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

void ProgrammerAsyncWrapper::unlockMemory() {
  initOperation("unlockMemory");

  ReturnStatus ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("unlockMemory", ret);
    return;
  }

  completeOperation("unlockMemory");
}

void ProgrammerAsyncWrapper::lockMemory() {
  initOperation("unlockMemory");

  ReturnStatus ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("unlockMemory", ret);
    return;
  }

  completeOperation("unlockMemory");
}
