#include "programmer_manager.h"
#include "global_environment.h"
#include "jlink_exe_programmer.h"
#include "log_system.h"

ProgrammerManager::ProgrammerManager(const QString& name)
    : AbstractManager{name} {
  loadSettings();
}

ProgrammerManager::~ProgrammerManager() {}

void ProgrammerManager::onInstanceThreadStarted() {
  createProgrammer();
}

AbstractManager::Type ProgrammerManager::type() const {
  return Type::Programmer;
}

void ProgrammerManager::applySettings() {
  sendLog("Применение новых настроек.");
  loadSettings();

  Programmer->applySettings();
}

void ProgrammerManager::programMemory(const std::shared_ptr<QString> path) {
  emit executionStarted("programMemory");
  sendLog("Загрузка программного обеспечения в память. ");

  // Создаем файл с данными
  ReturnStatus ret;
  QFile firmware(*path);

  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("programMemory", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Загрузка программного обеспечения успешно завершена. ");
  emit executionFinished("programMemory", ret);
}

void ProgrammerManager::readMemory() {
  emit executionStarted("readMemory");
  sendLog("Считывание памяти. ");

  ReturnStatus ret;
  ret = Programmer->readMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("readMemory", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Память успешно считана. ");
  emit executionFinished("readMemory", ret);
}

void ProgrammerManager::eraseMemory() {
  emit executionStarted("eraseMemory");
  sendLog("Очистка памяти. ");

  ReturnStatus ret;
  ret = Programmer->eraseMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("eraseMemory", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Память успешно очищена. ");
  emit executionFinished("eraseMemory", ret);
}

void ProgrammerManager::programUserData(const std::shared_ptr<QString> path) {
  emit executionStarted("programUserData");
  sendLog("Загрузка пользовательских данных в память. ");

  // Создаем файл с данными
  QFile dataFile(*path);
  ReturnStatus ret;

  ret = Programmer->programUserData(dataFile);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("programUserData", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Пользовательские данные успешно загружены. ");
  emit executionFinished("programUserData", ret);
}

void ProgrammerManager::readUserData() {
  emit executionStarted("readUserData");
  sendLog("Считывание пользовательских данных. ");

  ReturnStatus ret;
  ret = Programmer->readUserData();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("readUserData", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Пользовательские данные успешно считаны. ");
  emit executionFinished("readUserData", ret);
}

void ProgrammerManager::readUcid() {
  emit executionStarted("readUcid");
  sendLog("Считывание UCID. ");

  ReturnStatus ret;
  QString ucid;
  ret = Programmer->readUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("readUcid", ret);
    return;
  }

  // Завершаем операцию
  sendLog("UCID успешно считан. ");
  //  emit displayUcid_signal(ucid);
  emit executionFinished("readUcid", ret);
}

void ProgrammerManager::unlockMemory() {
  emit executionStarted("unlockMemory");
  sendLog("Разблокирование памяти микроконтроллера. ");

  ReturnStatus ret;
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("lockMemory", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Память микроконтроллера успешно разблокирована. ");
  emit executionFinished("unlockMemory", ret);
}

void ProgrammerManager::lockMemory() {
  emit executionStarted("lockMemory");
  sendLog("Блокирование памяти микроконтроллера. ");

  ReturnStatus ret;
  ret = Programmer->lockMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("lockMemory", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Память микроконтроллера успешно заблокирована. ");
  emit executionFinished("lockMemory", ret);
}

void ProgrammerManager::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

void ProgrammerManager::loadSettings() {}

void ProgrammerManager::createProgrammer() {
  Programmer = std::unique_ptr<AbstractProgrammer>(
      new JLinkExeProgrammer("JLinkExeProgrammer2"));
  connect(Programmer.get(), &AbstractProgrammer::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}
