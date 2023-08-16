#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject* parent) : QObject(parent) {
  setObjectName("FirmwareManager");

  Programmer = nullptr;
  ProgrammerThread = nullptr;

  FirmwareFileInfo = nullptr;
  FirmwareFile = nullptr;

  UserDataFileInfo = nullptr;
  UserDataFile = nullptr;

  ReadyIndicator = true;

  Client = new PersoClient(this);
  connect(Client, &PersoClient::logging, this, &FirmwareManager::proxyLogging);

  // Создаем программатор и среду его выполнения
  createProgrammerInstance();
}

FirmwareManager::~FirmwareManager() {
  delete FirmwareFileInfo;
  delete UserDataFileInfo;

  if (ProgrammerThread->isRunning()) {
    ProgrammerThread->quit();
    ProgrammerThread->wait();
  }

  if (ClientThread->isRunning()) {
    ClientThread->quit();
    ClientThread->wait();
  }
}

InterfaceProgrammer* FirmwareManager::programmer() const {
  return Programmer;
}

void FirmwareManager::performFirmwareLoading(const QString& path,
                                             bool unlockOption) {
  // Проверяем наличие файла
  processingFirmwarePath(path);

  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Вызываем соответствующую функцию
  if (unlockOption == true) {
    emit loadFirmware_signal();
  } else {
    emit loadFirmwareWithUnlock_signal();
  }
}

void FirmwareManager::performFirmwareReading() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Вызываем соответствующую функцию
  emit readFirmware_signal();
}

void FirmwareManager::performFirmwareErasing() {
  // Проверяем готовность
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Вызываем соответствующую функцию
  emit eraseFirmware_signal();
}

void FirmwareManager::performUserDataReading() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Создаем программатор и среду его выполнения
  createProgrammerInstance();

  // Когда поток будет запущен, Programmer начнет выполнять соответствующую
  // операцию
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::readUserData);

  // Вызываем соответствующую функцию
  emit readUserData_signal();
}

void FirmwareManager::performUserDataLoading(const QString& path) {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Проверяем наличие файла
  processingUserDataPath(path);

  // Вызываем соответствующую функцию
  emit loadUserData_signal();
}

void FirmwareManager::performDeviceUnlock() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Вызываем соответствующую функцию
  emit unlockDevice_signal();
}

void FirmwareManager::performDeviceLock() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  // Вызываем соответствующую функцию
  emit lockDevice_signal();
}

void FirmwareManager::performServerConnecting() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  Client->connectToPersoServer();

  ReadyIndicator = true;
}

void FirmwareManager::performServerDisconnecting() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  Client->disconnectFromPersoServer();

  ReadyIndicator = true;
}

void FirmwareManager::performServerEchoRequest() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  emit logging("Отправка эхо-запроса. ");
  Client->requestEcho();

  ReadyIndicator = true;
}

void FirmwareManager::performServerFirmwareRequest() {
  // Проверяем готовность к выполнению операции
  if (ReadyIndicator == false)
    return;
  else
    ReadyIndicator = false;

  emit logging("Отправка запроса на получение прошивки. ");
  Client->requestFirmware();

  ReadyIndicator = true;
}

void FirmwareManager::setFirmwareFile(const QString& path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(
        QString("The firmware file is missing in the default "
                "directory, select it manually."));
}

void FirmwareManager::applySettings() {
  Client->applySettings();
  Programmer->applySettings();
}

void FirmwareManager::processingFirmwarePath(const QString& path) {
  delete FirmwareFileInfo;
  delete FirmwareFile;

  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}

void FirmwareManager::processingUserDataPath(const QString& path) {
  delete UserDataFileInfo;
  delete UserDataFile;

  UserDataFileInfo = new QFileInfo(path);

  if ((UserDataFileInfo->exists()) && (UserDataFileInfo->isFile()))
    UserDataFile = new QFile(path, this);
  else
    UserDataFile = nullptr;
}

void FirmwareManager::createProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
  connect(Programmer, &InterfaceProgrammer::logging, this,
          &FirmwareManager::proxyLogging);
  // Когда поток завершит работу, объект Programmer будет удален
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ProgrammerThread, &QThread::finished, ProgrammerThread,
          &QObject::deleteLater);
  // Когда программатор завершит операцию, можно будет приступить к следующей
  connect(Programmer, &InterfaceProgrammer::operationFinished, this,
          &FirmwareManager::on_ProgrammerOperationFinished_slot);

  // Подключаем функционал
  connect(this, &FirmwareManager::loadFirmware_signal, Programmer,
          &InterfaceProgrammer::loadFirmware);
  connect(this, &FirmwareManager::loadFirmwareWithUnlock_signal, Programmer,
          &InterfaceProgrammer::loadFirmwareWithUnlock);
  connect(this, &FirmwareManager::readFirmware_signal, Programmer,
          &InterfaceProgrammer::readFirmware);
  connect(this, &FirmwareManager::eraseFirmware_signal, Programmer,
          &InterfaceProgrammer::eraseFirmware);
  connect(this, &FirmwareManager::loadUserData_signal, Programmer,
          &InterfaceProgrammer::loadUserData);
  connect(this, &FirmwareManager::readUserData_signal, Programmer,
          &InterfaceProgrammer::readUserData);
  connect(this, &FirmwareManager::unlockDevice_signal, Programmer,
          &InterfaceProgrammer::unlockDevice);
  connect(this, &FirmwareManager::lockDevice_signal, Programmer,
          &InterfaceProgrammer::lockDevice);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void FirmwareManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "JLinkExeProgrammer")
    emit logging("JLink.exe - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else
    emit logging("Unknown - " + log);
}

void FirmwareManager::on_ProgrammerOperationFinished_slot(
    PerfomingStatus status) {
  ReadyIndicator = true;
  LastStatus = status;

  if (LastStatus == FirmwareManager::Completed)
    emit notifyUser("Операция успешно завершена. ");
  else
    emit notifyUserAboutError("Выполнение операции провалено. ");
}
