#include "programmator_manager.h"

ProgrammatorManager::ProgrammatorManager(QObject *parent) : QObject(parent) {
  Programmer = nullptr;
  ProgrammerThread = nullptr;
  Logger = nullptr;

  FirmwareFileInfo = nullptr;
  FirmwareFile = nullptr;

  ReadyIndicator = true;
}

ProgrammatorManager::~ProgrammatorManager() {
  delete FirmwareFileInfo;

  if (ReadyIndicator == false) {
    ProgrammerThread->quit();
    ProgrammerThread->wait();
  }
}

InterfaceProgrammer *ProgrammatorManager::programmer() const {
  return Programmer;
}

void ProgrammatorManager::performFirmwareErasing() {
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

void ProgrammatorManager::performFirmwareLoading(const QString &path) {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Проверяем наличие файла
  processingFirmwarePath(path);

  // Создаем программатор и среду его выполнения
  buildProgrammerInstance();

  // Отправляем прошивку программатору
  Programmer->setLoadingFirmware(FirmwareFile);

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::loadFirmware);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammatorManager::performUserDataLoading(const QString &path) {
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

void ProgrammatorManager::performDeviceUnlock() {
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
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::unlockDevice);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ProgrammatorManager::performDeviceFirmwareReading() {
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

void ProgrammatorManager::performDeviceUserDataReading() {
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

void ProgrammatorManager::setFirmwareFile(const QString &path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(QString("The firmware file is missing in the default "
                         "directory, select it manually."));
}

void ProgrammatorManager::setLogger(LogSystem *logger) { Logger = logger; }

void ProgrammatorManager::processingFirmwarePath(const QString &path) {
  delete FirmwareFileInfo;
  delete FirmwareFile;

  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}

void ProgrammatorManager::processingUserDataPath(const QString &path) {
  delete UserDataFileInfo;
  delete UserDataFile;

  UserDataFileInfo = new QFileInfo(path);

  if ((UserDataFileInfo->exists()) && (UserDataFileInfo->isFile()))
    UserDataFile = new QFile(path, this);
  else
    UserDataFile = nullptr;
}

void ProgrammatorManager::buildProgrammerInstance() {
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
          &ProgrammatorManager::performingFinished);
  // Если программатор успешно выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationCompleted, this,
          &ProgrammatorManager::performingCompleted);
  // Если программатор не выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationFailed, this,
          &ProgrammatorManager::performingFailed);
}

void ProgrammatorManager::performingFinished() {
  ReadyIndicator = true;
  notifyUser(LastStatus);
}

void ProgrammatorManager::performingCompleted() { LastStatus = Completed; }

void ProgrammatorManager::performingFailed() { LastStatus = Failed; }
