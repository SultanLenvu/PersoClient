#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject* parent) : QObject(parent) {
  setObjectName("FirmwareManager");

  // Создаем программатор и среду его выполнения
  createProgrammerInstance();

  // Создаем клиент и среду его выполнения
  createClientInstance();

  // Создаем цикл ожидания
  createWaitingLoop();

  // Создаем таймеры для работы с временем
  createTimers();

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

  delete ODMeter;
}

IProgrammer* FirmwareManager::programmer() const {
  return Programmer;
}

void FirmwareManager::performFirmwareLoading(const QString& path,
                                             bool unlockOption) {
  // Начинаем операцию
  if (!startOperationExecution("performFirmwareLoading")) {
    return;
  }

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

  // Завершаем операцию
  endOperationExecution("performFirmwareLoading");
}

void FirmwareManager::performFirmwareReading() {
  // Начинаем операцию
  if (!startOperationExecution("performFirmwareReading")) {
    return;
  }

  emit logging("Считывание прошивки микроконтроллера. ");
  emit readFirmware_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performFirmwareReading");
}

void FirmwareManager::performFirmwareErasing() {
  // Начинаем операцию
  if (!startOperationExecution("performFirmwareErasing")) {
    return;
  }

  emit logging("Стирание прошивки микроконтроллера. ");
  emit eraseFirmware_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performFirmwareErasing");
}

void FirmwareManager::performDataReading() {
  // Начинаем операцию
  if (!startOperationExecution("performDataReading")) {
    return;
  }

  emit logging("Считывание данных в память микроконтроллера. ");
  emit readData_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performDataReading");
}

void FirmwareManager::performDataLoading(const QString& path) {
  // Начинаем операцию
  if (!startOperationExecution("performDataLoading")) {
    return;
  }

  // Создаем файл с данными
  QFile dataFile(path);

  emit logging("Загрузка данных в память микроконтроллера. ");
  emit loadData_signal(&dataFile);

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performDataLoading");
}

void FirmwareManager::performDeviceUnlock() {
  // Начинаем операцию
  if (!startOperationExecution("performDeviceUnlock")) {
    return;
  }

  emit logging("Разблокирование памяти микроконтроллера. ");
  emit unlockDevice_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performDeviceUnlock");
}

void FirmwareManager::performDeviceLock() {
  // Начинаем операцию
  if (!startOperationExecution("performDeviceLock")) {
    return;
  }

  emit logging("Блокирование памяти микроконтроллера. ");
  emit lockDevice_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performDeviceLock");
}

void FirmwareManager::performServerConnecting() {
  // Начинаем операцию
  if (!startOperationExecution("performServerConnecting")) {
    return;
  }

  emit logging("Подключение к серверу персонализации. ");
  emit connectToPersoServer_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performServerConnecting");
}

void FirmwareManager::performServerDisconnecting() {
  // Начинаем операцию
  if (!startOperationExecution("performServerDisconnecting")) {
    return;
  }

  emit logging("Отключение от сервера персонализации. ");
  emit disconnectFromPersoServer_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performServerDisconnecting");
}

void FirmwareManager::performServerEchoRequest() {
  // Начинаем операцию
  if (!startOperationExecution("performServerEchoRequest")) {
    return;
  }

  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  emit requestEcho_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performServerEchoRequest");
}

void FirmwareManager::performServerFirmwareRequest() {
  // Начинаем операцию
  if (!startOperationExecution("performServerFirmwareRequest")) {
    return;
  }

  // Создаем файл прошивки
  QFile firmware("firmware.hex", this);

  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(&firmware);

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performServerFirmwareRequest");
}

void FirmwareManager::performServerFirmwareLoading() {
  // Начинаем операцию
  if (!startOperationExecution("performServerFirmwareLoading")) {
    return;
  }

  // Создаем файл прошивки
  QFile firmware("firmware.hex", this);

  // Запрашиваем файл прошивки у сервера
  emit logging("Отправка запроса на получение прошивки. ");
  emit requestFirmware_signal(&firmware);

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Оповещаем пользователя о результатах
  if (CurrentState != Completed) {
    // Завершаем операцию
    endOperationExecution("performServerFirmwareLoading");
    return;
  }

  emit logging("Разблокирование памяти и загрузка прошивки микроконтроллера. ");
  emit loadFirmwareWithUnlock_signal(&firmware);

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingExecution;

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Удаляем файл прошивки
  // firmware.remove();

  // Завершаем операцию
  endOperationExecution("performServerFirmwareLoading");
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

void FirmwareManager::createClientInstance() {
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

void FirmwareManager::createTimers() {
  // Таймер, отслеживающий длительность выполняющихся операций
  ODTimer = new QTimer(this);
  ODTimer->setInterval(FIRMWARE_MANAGER_OPERATION_MAX_DURATION);
  connect(ODTimer, &QTimer::timeout, this,
          &FirmwareManager::on_ODTimerTimeout_slot);
  connect(ODTimer, &QTimer::timeout, ODTimer, &QTimer::stop);
  connect(this, &FirmwareManager::operationPerformingEnded, ODTimer,
          &QTimer::stop);

  // Таймер для измерения длительности операции
  ODMeter = new QElapsedTimer();
}

void FirmwareManager::setupODQTimer(uint32_t msecs) {
  // Таймер, отслеживающий квант длительности операции
  ODQTimer = new QTimer(this);
  ODQTimer->setInterval(msecs);

  connect(ODQTimer, &QTimer::timeout, this,
          &FirmwareManager::on_ODQTimerTimeout_slot);
  connect(this, &FirmwareManager::operationPerformingEnded, ODQTimer,
          &QTimer::stop);
}

bool FirmwareManager::startOperationExecution(const QString& operationName) {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready)
    return false;

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingExecution;

  // Настраиваем и запускаем таймер для измерения квантов времени
  QSettings settings;
  uint64_t operationDuration =
      settings
          .value(QString("FirmwareManager/Operations/") + operationName +
                 QString("/Duration"))
          .toInt();
  uint32_t operationQuantDuration = operationDuration / 100;
  operationQuantDuration++;
  emit logging(QString("Длительность кванта операции: %1.")
                   .arg(QString::number(operationQuantDuration)));
  setupODQTimer(operationQuantDuration);
  ODQTimer->start();

  // Запускаем таймер для контроля максимальной длительности операции
  ODTimer->start();

  // Запускаем измеритель длительности операции
  ODMeter->start();

  // Отправляем сигнал о начале выполнения длительной операции
  emit operationPerfomingStarted();

  return true;
}

void FirmwareManager::endOperationExecution(const QString& operationName) {
  QSettings settings;

  // Измеряем и сохраняем длительность операции
  uint64_t duration = ODMeter->elapsed();
  emit logging(
      QString("Длительность операции: %1.").arg(QString::number(duration)));
  settings.setValue(QString("FirmwareManager/Operations/") + operationName +
                        QString("/Duration"),
                    duration);

  // Сигнал о завершении текущей операции
  emit operationPerformingEnded();

  // Оповещаем пользователя о результатах
  if (CurrentState == Completed) {
    emit notifyUser(NotificarionText);
  } else {
    emit notifyUserAboutError(NotificarionText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void FirmwareManager::deleteClientInstance() {
  if (!ClientThread->isRunning())
    return;

  ClientThread->quit();
  ClientThread->wait();
  delete ClientThread;
  delete Client;
}

void FirmwareManager::deleteProgrammerInstance() {
  if (!ProgrammerThread->isRunning())
    return;

  ProgrammerThread->quit();
  ProgrammerThread->wait();
  delete ProgrammerThread;
  delete Programmer;
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
    IProgrammer::ExecutionStatus status) {
  switch (status) {
    case IProgrammer::NotExecuted:
      CurrentState = Failed;
      NotificarionText = "Программатор: операция не была запущена. ";
      emit break;
    case IProgrammer::FirmwareFileError:
      CurrentState = Failed;
      NotificarionText = "Программатор: некорректный файл прошивки. ";
      break;
    case IProgrammer::DataFileError:
      CurrentState = Failed;
      NotificarionText =
          "Программатор: некорректный файл данных для загрузки. ";
      break;
    case IProgrammer::ProgrammatorError:
      CurrentState = Failed;
      NotificarionText =
          "Программатор: получена ошибка при выполнении операции. ";
      break;
    case IProgrammer::CompletedSuccessfully:
      CurrentState = Completed;
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
      CurrentState = Failed;
      NotificarionText = "Клиент: операция не была запущена. ";
      emit break;
    case PersoClient::FirmwareFileError:
      CurrentState = Failed;
      NotificarionText = "Клиент: некорректный файл прошивки. ";
      break;
    case PersoClient::ServerConnectionError:
      CurrentState = Failed;
      NotificarionText = "Клиент: не удалось подключиться к серверу. ";
      break;
    case PersoClient::ServerNotResponding:
      CurrentState = Failed;
      NotificarionText = "Клиент: сервер не отвечает. ";
      break;
    case PersoClient::ServerConnectionTerminated:
      CurrentState = Failed;
      NotificarionText = "Клиент: соединение с сервером прервалось. ";
      break;
    case PersoClient::ResponseProcessingError:
      CurrentState = Failed;
      NotificarionText = "Клиент: получен некорректный ответ от сервера. ";
      break;
    case PersoClient::UnknownError:
      CurrentState = Failed;
      NotificarionText = "Клиент: неизвестная ошибка. ";
      break;
    case PersoClient::CompletedSuccessfully:
      CurrentState = Completed;
      NotificarionText = "Операция успешно выполнена. ";
      break;
  }

  // Завершаем цикл ожидания
  emit waitingEnd();
}

void FirmwareManager::on_ODTimerTimeout_slot() {
  emit logging("Операция выполняется слишком долго. Сброс. ");
  emit notifyUserAboutError("Операция выполняется слишком долго. Сброс. ");

  deleteClientInstance();
  createClientInstance();

  deleteProgrammerInstance();
  createProgrammerInstance();
}

void FirmwareManager::on_ODQTimerTimeout_slot() {
  emit operationStepPerfomed();
}
