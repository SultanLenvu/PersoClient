#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject* parent) : QObject(parent) {
  setObjectName("FirmwareManager");

  FirmwareFile = nullptr;
  DataFile = nullptr;

  ReadyIndicator = true;

  // Создаем цикл ожидания
  WaitingLoop = new QEventLoop(this);

  // Создаем программатор и среду его выполнения
  createProgrammerInstance();

  // Создаем клиент и среду его выполнения
  creareClientInstance();

  // Инициализируем состояние
  State = Ready;

  // Регистрация пользовательского типа данных
  qRegisterMetaType<OperationStatus>("OperationStatus");
}

FirmwareManager::~FirmwareManager() {
  if (ProgrammerThread->isRunning()) {
    ProgrammerThread->quit();
    ProgrammerThread->wait();
  }

  if (ClientThread->isRunning()) {
    ClientThread->quit();
    ClientThread->wait();
  }

  deleteFirmwareFile();
  deleteDataFile();
}

InterfaceProgrammer* FirmwareManager::programmer() const {
  return Programmer;
}

void FirmwareManager::performFirmwareLoading(const QString& path,
                                             bool unlockOption) {
  // Создаем файл
  FirmwareFile = new QFile(path, this);

  // Вызываем соответствующую функцию
  if (unlockOption == true) {
    emit logging("Загрузка прошивки микроконтроллера. ");
    emit loadFirmware_signal(FirmwareFile);
  } else {
    emit logging(
        "Разблокирование памяти и загрузка прошивки микроконтроллера. ");
    emit loadFirmwareWithUnlock_signal(FirmwareFile);
  }
}

void FirmwareManager::performFirmwareReading() {
  emit logging("Считывание прошивки микроконтроллера. ");
  emit readFirmware_signal();
}

void FirmwareManager::performFirmwareErasing() {
  emit logging("Стирание прошивки микроконтроллера. ");
  emit eraseFirmware_signal();
}

void FirmwareManager::performDataReading() {
  emit logging("Считывание данных в память микроконтроллера. ");
  emit readData_signal();
}

void FirmwareManager::performDataLoading(const QString& path) {
  // Создаем файл с данными
  DataFile = new QFile(path, this);

  emit logging("Загрузка данных в память микроконтроллера. ");
  emit loadData_signal(DataFile);
}

void FirmwareManager::performDeviceUnlock() {
  emit logging("Разблокирование памяти микроконтроллера. ");
  emit unlockDevice_signal();
}

void FirmwareManager::performDeviceLock() {
  emit logging("Блокирование памяти микроконтроллера. ");
  emit lockDevice_signal();
}

void FirmwareManager::performServerConnecting() {
  emit logging("Подключение к серверу персонализации. ");
  emit connectToPersoServer_signal();
}

void FirmwareManager::performServerDisconnecting() {
  emit logging("Отключение от сервера персонализации. ");
  emit disconnectFromPersoServer_signal();
}

void FirmwareManager::performServerEchoRequest() {
  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  emit requestEcho_signal();
}

void FirmwareManager::performServerFirmwareRequest() {
  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(FirmwareFile);
}

void FirmwareManager::performServerFirmwareLoading() {
  // Создаем файл
  FirmwareFile = new QFile("firmware.hex", this);

  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(FirmwareFile);

  emit logging("Разблокирование памяти и загрузка прошивки микроконтроллера. ");
  emit loadFirmwareWithUnlock_signal(FirmwareFile);
}

void FirmwareManager::applySettings() {
  Client->applySettings();
  Programmer->applySettings();
}

void FirmwareManager::deleteFirmwareFile() {
  if (!FirmwareFile)
    return;

  if (FirmwareFile->exists()) {
    FirmwareFile->remove();
  }

  delete FirmwareFile;
  FirmwareFile = nullptr;

  emit logging("Файл прошивки удален. ");
}

void FirmwareManager::deleteDataFile() {
  if (!DataFile)
    return;

  if (DataFile->exists()) {
    DataFile->remove();
  }

  delete DataFile;
  DataFile = nullptr;

  emit logging("Файл данных удален. ");
}

void FirmwareManager::createProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer(nullptr);
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
  connect(this, &FirmwareManager::loadData_signal, Programmer,
          &InterfaceProgrammer::loadData);
  connect(this, &FirmwareManager::readData_signal, Programmer,
          &InterfaceProgrammer::readData);
  connect(this, &FirmwareManager::unlockDevice_signal, Programmer,
          &InterfaceProgrammer::unlockDevice);
  connect(this, &FirmwareManager::lockDevice_signal, Programmer,
          &InterfaceProgrammer::lockDevice);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void FirmwareManager::creareClientInstance() {
  // Создаем поток для программатора
  ClientThread = new QThread(this);

  // И интерфейс программатора
  Client = new PersoClient(nullptr);
  Client->moveToThread(ClientThread);

  // Подключаем логгирование к клиенту
  connect(Client, &PersoClient::logging, this, &FirmwareManager::proxyLogging);
  // Когда поток завершит работу, клиент будет удален
  connect(ClientThread, &QThread::finished, Programmer, &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ClientThread, &QThread::finished, ClientThread,
          &QObject::deleteLater);
  // Когда клиент завершит операцию, можно будет приступить к следующей
  connect(Client, &PersoClient::operationFinished, this,
          &FirmwareManager::on_ClientOperationFinished_slot);

  // Подключаем функционал
  connect(this, &FirmwareManager::connectToPersoServer_signal, Client,
          &PersoClient::connectToPersoServer);
  connect(this, &FirmwareManager::disconnectFromPersoServer_signal, Client,
          &PersoClient::disconnectFromPersoServer);
  connect(this, &FirmwareManager::requestEcho_signal, Client,
          &PersoClient::requestEcho);
  connect(this, &FirmwareManager::requestFirmware_signal, Client,
          &PersoClient::requestFirmware);

  // Запускаем поток клиента
  ClientThread->start();
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
    OperationStatus status) {
  if (status == OperationStatus::Success) {
    emit logging("Операция программатора успешно выполнена. ");
    emit notifyUser("Операция успешно выполнена. ");
  } else {
    emit logging("Операция программатора провалена. ");
    emit notifyUserAboutError("Выполнение операции провалено. ");
  }
}

void FirmwareManager::on_ClientOperationFinished_slot(OperationStatus status) {
  if (status == OperationStatus::Success) {
    emit logging("Операция программатора успешно выполнена. ");
    emit notifyUser("Операция успешно выполнена. ");
  } else {
    emit logging("Операция программатора провалена. ");
    emit notifyUserAboutError("Выполнение операции провалено. ");
  }
}
