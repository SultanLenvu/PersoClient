#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject *parent, LogSystem *logger)
    : QObject(parent) {
  Logger = logger;

  Programmer = nullptr;
  ProgrammerThread = nullptr;

  ReadyIndicator = true;

  processingFirmwarePath(DEFAULT_FIRMWARE_FILE_PATH);
}

FirmwareManager::~FirmwareManager() {
  delete FirmwareFileInfo;

  if (ProgrammerThread) {
    if (ProgrammerThread->isFinished() == false) {
      ProgrammerThread->quit();
      ProgrammerThread->wait();
    }
  }
}

InterfaceProgrammer *FirmwareManager::programmer() const { return Programmer; }

void FirmwareManager::performErasing() {
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

void FirmwareManager::performLoading() {
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
          &InterfaceProgrammer::loadFirmware);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void FirmwareManager::performAutoLoading() {}

void FirmwareManager::setFirmwareFile(const QString &path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(QString("The firmware file is missing in the default "
                         "directory, select it manually."));
}

void FirmwareManager::processingFirmwarePath(const QString &path) {
  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}

void FirmwareManager::buildProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
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
          &FirmwareManager::performingFinished);
  // Если программатор успешно выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationCompleted, this,
          &FirmwareManager::performingCompleted);
  // Если программатор не выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationFailed, this,
          &FirmwareManager::performingFailed);
}

void FirmwareManager::performingFinished() {
  ReadyIndicator = true;
  notifyUser(LastStatus);
}

void FirmwareManager::performingCompleted() { LastStatus = Completed; }

void FirmwareManager::performingFailed() { LastStatus = Failed; }
