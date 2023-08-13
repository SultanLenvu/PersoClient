#include "client_manager.h"

ClientManager::ClientManager(QObject* parent) : QObject(parent) {
  Programmer = nullptr;
  ProgrammerThread = nullptr;

  FirmwareFileInfo = nullptr;
  FirmwareFile = nullptr;

  UserDataFileInfo = nullptr;
  UserDataFile = nullptr;

  ReadyIndicator = true;

  Client = new PersoClient(this);
  connect(Client, &PersoClient::logging, this, &ClientManager::proxyLogging);
}

ClientManager::~ClientManager() {
  delete FirmwareFileInfo;
  delete UserDataFileInfo;

  if (ReadyIndicator == false) {
    ProgrammerThread->quit();
    ProgrammerThread->wait();
  }
}

InterfaceProgrammer* ClientManager::programmer() const {
  return Programmer;
}

void ClientManager::performFirmwareLoading(const QString& path,
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

void ClientManager::performFirmwareReading() {
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

void ClientManager::performFirmwareErasing() {
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

void ClientManager::performUserDataLoading(const QString& path) {
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

void ClientManager::performDeviceUnlock() {
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

void ClientManager::performDeviceLock() {
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

void ClientManager::performServerConnecting() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  Client->connectToPersoServer();

  ReadyIndicator = true;
}

void ClientManager::performServerDisconnecting() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  Client->disconnectFromPersoServer();

  ReadyIndicator = true;
}

void ClientManager::performServerEchoRequest() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  emit logging("Отправка эхо-запроса. ");
  Client->sendEchoRequest();

  ReadyIndicator = true;
}

void ClientManager::performUserDataReading() {
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

void ClientManager::setFirmwareFile(const QString& path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(
        QString("The firmware file is missing in the default "
                "directory, select it manually."));
}

void ClientManager::processingFirmwarePath(const QString& path) {
  delete FirmwareFileInfo;
  delete FirmwareFile;

  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}

void ClientManager::processingUserDataPath(const QString& path) {
  delete UserDataFileInfo;
  delete UserDataFile;

  UserDataFileInfo = new QFileInfo(path);

  if ((UserDataFileInfo->exists()) && (UserDataFileInfo->isFile()))
    UserDataFile = new QFile(path, this);
  else
    UserDataFile = nullptr;
}

void ClientManager::buildProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
  connect(Programmer, &InterfaceProgrammer::logging, this,
          &ClientManager::proxyLogging);
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
          &ClientManager::performingFinished);
  // Если программатор успешно выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationCompleted, this,
          &ClientManager::performingCompleted);
  // Если программатор не выполнит свою операцию
  connect(Programmer, &InterfaceProgrammer::operationFailed, this,
          &ClientManager::performingFailed);
}

void ClientManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "JLinkExeProgrammer")
    emit logging("JLink.exe - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else
    emit logging("Unknown - " + log);
}

void ClientManager::performingFinished() {
  ReadyIndicator = true;

  if (LastStatus == ClientManager::Completed)
    emit notifyUser("Операция успешно завершена. ");
  else
    emit notifyUserAboutError("Выполнение операции провалено. ");
}

void ClientManager::performingCompleted() {
  LastStatus = Completed;
}

void ClientManager::performingFailed() {
  LastStatus = Failed;
}
