#include "programmer_manager.h"

ProgrammerManager::ProgrammerManager(QObject *parent) : QObject(parent) {
  Programmer = nullptr;
  ProgrammerThread = nullptr;
  Logger = nullptr;

  FirmwareFileInfo = nullptr;
  FirmwareFile = nullptr;

  UserDataFileInfo = nullptr;
  UserDataFile = nullptr;

  ReadyIndicator = true;
}

ProgrammerManager::~ProgrammerManager() {
  delete FirmwareFileInfo;
  delete UserDataFileInfo;

  if (ReadyIndicator == false) {
    ProgrammerThread->quit();
    ProgrammerThread->wait();
  }
}

InterfaceProgrammer *ProgrammerManager::programmer() const {
  return Programmer;
}

void ProgrammerManager::performFirmwareLoading(const QString &path,
                                               bool unlockOption) {
  // Проверяем наличие файла
  processingFirmwarePath(path);

  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Отправляем прошивку программатору
  Programmer->setLoadingFirmware(FirmwareFile);

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  if (unlockOption == true)
    connect(ProgrammerThread, &QThread::started, Programmer,
            &InterfaceProgrammer::loadFirmwareWithUnlock);
  else
    connect(ProgrammerThread, &QThread::started, Programmer,
            &InterfaceProgrammer::loadFirmware);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performFirmwareReading() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::readFirmware);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performFirmwareErasing() {
  // Проверяем готовность
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::eraseFirmware);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performUserDataLoading(const QString &path) {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Проверяем наличие файла
  processingUserDataPath(path);

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Отправляем прошивку программатору
  Programmer->setLoadingUserData(UserDataFile);

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::loadUserData);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performDeviceUnlock() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::unlockDevice);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performDeviceLock() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::lockDevice);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::performUserDataReading() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::readUserData);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammerManager::setFirmwareFile(const QString &path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(QString("The firmware file is missing in the default "
                         "directory, select it manually."));
}

void ProgrammerManager::setLogger(LogSystem *logger) { Logger = logger; }

void ProgrammerManager::processingFirmwarePath(const QString &path) {
  delete FirmwareFileInfo;
  delete FirmwareFile;

  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}

void ProgrammerManager::processingUserDataPath(const QString &path) {
  delete UserDataFileInfo;
  delete UserDataFile;

  UserDataFileInfo = new QFileInfo(path);

  if ((UserDataFileInfo->exists()) && (UserDataFileInfo->isFile()))
    UserDataFile = new QFile(path, this);
  else
    UserDataFile = nullptr;
}

void ProgrammerManager::buildProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
  if (Logger != nullptr)
    connect(Programmer, &InterfaceProgrammer::logging, Logger,
            &LogSystem::programmerLog);
  // Когда объект Programmer завершит свою работу, поток начнет свое
  // завершение
  connect(Programmer, &InterfaceProgrammer::operationFinished, ProgrammerThread,
          &QThread::quit);
  // Когда поток завершит работу, объект Programmer будет удален
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ProgrammerThread, &QThread::finished, ProgrammerThread,
          &QObject::deleteLater);
  // Когда программатор завершит операцию, можно будет приступить к следующей
  connect(Programmer, &InterfaceProgrammer::operationFinished, this,
          &ProgrammerManager::performingFinished);
  // Если программатор успешно выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationCompleted, this,
          &ProgrammerManager::performingCompleted);
  // Если программатор не выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationFailed, this,
          &ProgrammerManager::performingFailed);
}

void ProgrammerManager::performingFinished() {
  ReadyIndicator = true;
  notifyUser(LastStatus);
}

void ProgrammerManager::performingCompleted() { LastStatus = Completed; }

void ProgrammerManager::performingFailed() { LastStatus = Failed; }
