#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject* parent) : QObject(parent) {
  setObjectName("FirmwareManager");

  ReadyIndicator = true;

  // Создаем программатор и среду его выполнения
  createProgrammerInstance();

  // Создаем клиент и среду его выполнения
  creareClientInstance();

  // Создаем цикл ожидания
  createWaitingLoop();

  // Инициализируем текущее состояние
  CurrentState = Ready;
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
}

IProgrammer* FirmwareManager::programmer() const {
  return Programmer;
}

void FirmwareManager::performFirmwareLoading(const QString& path,
                                             bool unlockOption) {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  // Создаем файл
  QFile firmware(path);

  // Вызываем соответствующую функцию
  if (unlockOption == true) {
    emit logging("Загрузка прошивки микроконтроллера. ");
    emit loadFirmware_signal(&firmware);
  } else {
    emit logging(
        "Разблокирование памяти и загрузка прошивки микроконтроллера. ");
    emit loadFirmwareWithUnlock_signal(&firmware);
  }

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performFirmwareReading() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Считывание прошивки микроконтроллера. ");
  emit readFirmware_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performFirmwareErasing() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Стирание прошивки микроконтроллера. ");
  emit eraseFirmware_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performDataReading() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Считывание данных в память микроконтроллера. ");
  emit readData_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performDataLoading(const QString& path) {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  // Создаем файл с данными
  QFile dataFile(path);

  emit logging("Загрузка данных в память микроконтроллера. ");
  emit loadData_signal(&dataFile);

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performDeviceUnlock() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Разблокирование памяти микроконтроллера. ");
  emit unlockDevice_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performDeviceLock() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Блокирование памяти микроконтроллера. ");
  emit lockDevice_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performServerConnecting() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Подключение к серверу персонализации. ");
  emit connectToPersoServer_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ClientCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performServerDisconnecting() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Отключение от сервера персонализации. ");
  emit disconnectFromPersoServer_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ClientCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performServerEchoRequest() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  emit requestEcho_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ClientCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performServerFirmwareRequest() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  // Создаем файл прошивки
  QFile firmware("firmware.hex", this);

  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(&firmware);

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ClientCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::performServerFirmwareLoading() {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingClientProcessing;

  // Создаем файл прошивки
  QFile firmware("firmware.hex", this);

  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(&firmware);

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState != ClientCompleted) {
    emit notifyUserAboutError(NotificarionText);
    return;
  }

  emit logging("Разблокирование памяти и загрузка прошивки микроконтроллера. ");
  emit loadFirmwareWithUnlock_signal(&firmware);

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingProgrammerProcessing;

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState == ProgrammerCompleted) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Удаляем файл прошивки
  // firmware.remove();

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::applySettings() {
  Client->applySettings();
  Programmer->applySettings();
}

void FirmwareManager::createProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer(nullptr);
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
  connect(Programmer, &IProgrammer::logging, this,
          &FirmwareManager::proxyLogging);
  // Когда поток завершит работу, объект Programmer будет удален
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ProgrammerThread, &QThread::finished, ProgrammerThread,
          &QObject::deleteLater);
  // Когда программатор завершит операцию, можно будет приступить к следующей
  connect(Programmer, &IProgrammer::operationFinished, this,
          &FirmwareManager::on_ProgrammerOperationFinished_slot);

  // Подключаем функционал
  connect(this, &FirmwareManager::loadFirmware_signal, Programmer,
          &IProgrammer::loadFirmware);
  connect(this, &FirmwareManager::loadFirmwareWithUnlock_signal, Programmer,
          &IProgrammer::loadFirmwareWithUnlock);
  connect(this, &FirmwareManager::readFirmware_signal, Programmer,
          &IProgrammer::readFirmware);
  connect(this, &FirmwareManager::eraseFirmware_signal, Programmer,
          &IProgrammer::eraseFirmware);
  connect(this, &FirmwareManager::loadData_signal, Programmer,
          &IProgrammer::loadData);
  connect(this, &FirmwareManager::readData_signal, Programmer,
          &IProgrammer::readData);
  connect(this, &FirmwareManager::unlockDevice_signal, Programmer,
          &IProgrammer::unlockDevice);
  connect(this, &FirmwareManager::lockDevice_signal, Programmer,
          &IProgrammer::lockDevice);

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

void FirmwareManager::createWaitingLoop() {
  WaitingLoop = new QEventLoop(this);
  connect(this, &FirmwareManager::waitingEnd, WaitingLoop, &QEventLoop::quit);
}

void FirmwareManager::processingProgrammerStatus(
    IProgrammer::ExecutionStatus status) {}

void FirmwareManager::processingClientStatus(
    PersoClient::ExecutionStatus status) {}

void FirmwareManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "JLinkExeProgrammer")
    emit logging("JLink.exe - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else
    emit logging("Unknown - " + log);
}

void FirmwareManager::on_ProgrammerOperationFinished_slot(
    IProgrammer::ExecutionStatus status) {
  switch (status) {
    case IProgrammer::NotExecuted:
      CurrentState = ProgrammerFailed;
      NotificarionText = "Ошибка программатора: операция не была запущена. ";
      emit break;
    case IProgrammer::FirmwareFileError:
      CurrentState = ProgrammerFailed;
      NotificarionText = "Программатор: некорректный файл прошивки. ";
      break;
    case IProgrammer::DataFileError:
      CurrentState = ProgrammerFailed;
      NotificarionText =
          "Программатор: некорректный файл данных для загрузки. ";
      break;
    case IProgrammer::ProgrammatorError:
      CurrentState = ProgrammerFailed;
      NotificarionText =
          "Программатор: получена ошибка при выполнении операции. ";
      break;
    case IProgrammer::CompletedSuccessfully:
      CurrentState = ProgrammerCompleted;
      NotificarionText = "Операция успешно выполнена. ";
      break;
  }

  // Завершаем цикл ожидания
  emit waitingEnd();
}

void FirmwareManager::on_ClientOperationFinished_slot(
    PersoClient::ExecutionStatus status) {
  switch (status) {
    case PersoClient::NotExecuted:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: операция не была запущена. ";
      emit break;
    case PersoClient::FirmwareFileError:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: некорректный файл прошивки. ";
      break;
    case PersoClient::ServerConnectionError:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: не удалось подключиться к серверу. ";
      break;
    case PersoClient::ServerNotResponding:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: сервер не отвечает. ";
      break;
    case PersoClient::ServerConnectionTerminated:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: соединение с сервером прервалось. ";
      break;
    case PersoClient::ResponseProcessingError:
      CurrentState = ClientFailed;
      NotificarionText = "Клиент: получен некорректный ответ от сервера. ";
      break;
    case PersoClient::UnknownError:
      CurrentState = ClientCompleted;
      NotificarionText = "Клиент: неизвестная ошибка. ";
      break;
    case PersoClient::CompletedSuccessfully:
      CurrentState = ClientCompleted;
      NotificarionText = "Операция успешно выполнена. ";
      break;
  }

  // Завершаем цикл ожидания
  emit waitingEnd();
}
