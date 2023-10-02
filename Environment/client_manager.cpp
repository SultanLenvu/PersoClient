#include "client_manager.h"

ClientManager::ClientManager(QObject* parent) : QObject(parent) {
  setObjectName("ClientManager");

  // Создаем программатор
  createProgrammerInstance();

  // Создаем клиент
  createClientInstance();

  // Создаем принтер
  createPrinterInstance();

  // Создаем цикл ожидания
  createWaitingLoop();

  // Создаем таймеры для работы с временем
  createTimers();

  // Инициализируем текущее состояние
  CurrentState = Ready;

  CurrentLogin = PRODUCTION_LINE_DEFAULT_LOGIN;
  CurrentPassword = PRODUCTION_LINE_DEFAULT_PASSWORD;
}

ClientManager::~ClientManager() {
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

IProgrammer* ClientManager::programmer() const {
  return Programmer;
}

void ClientManager::performServerConnecting() {
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

void ClientManager::performServerDisconnecting() {
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

void ClientManager::performServerEcho() {
  // Начинаем операцию
  if (!startOperationExecution("performServerEcho")) {
    return;
  }

  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  emit requestEcho_signal();

  // Запуск цикла ожидания
  WaitingLoop->exec();

  // Завершаем операцию
  endOperationExecution("performServerEcho");
}

void ClientManager::performServerAuthorization(
    const QMap<QString, QString>* data,
    bool& result) {
  // Начинаем операцию
  if (!startOperationExecution("performServerAuthorization")) {
    return;
  }

  emit logging("Авторизация на сервере персонализации. ");
  emit requestAuthorize_signal(data);

  // Запуск цикла ожидания
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при авторизации на сервере. ");
    endOperationExecution("performServerAuthorization");
    return;
  }

  // Сохраняем данные и вызываем производственный интерфейс
  CurrentLogin = data->value("login");
  CurrentPassword = data->value("password");

  emit createProductionInterface_signal();

  // Формируем регзультат
  if (CurrentState == Completed) {
    result = true;
  } else {
    result = false;
  }

  // Завершаем операцию
  endOperationExecution("performServerAuthorization");
}

void ClientManager::performTransponderFirmwareLoading(
    TransponderInfoModel* model) {
  QString ucid;
  QMap<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);

  // Начинаем операцию
  if (!startOperationExecution("performTransponderFirmwareLoading")) {
    return;
  }

  emit logging("Разблокирование памяти транспондера. ");
  emit unlockDevice_signal();
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при разблокировании памяти транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  emit getUcid_signal(&ucid);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при считывании UCID с транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("ucid", ucid);
  emit logging("Запрос прошивки транспондера. ");
  emit requestTransponderRelease_signal(&requestParameters, &firmware);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при выпуске транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  emit loadFirmware_signal(&firmware);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  emit logging(
      "Отправка запроса на подтверждение загрузки прошивки в транспондер. ");
  emit requestTransponderReleaseConfirm_signal(&requestParameters,
                                               responseParameters);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при подтверждении выпуска транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  //  firmware.remove();

  emit logging("Печать стикера для транспондера.");
  if (!Printer->printTransponderSticker(responseParameters)) {
    emit logging("Получена ошибка при печати стикера для транспондера.");
    CurrentState = Failed;
    NotificationText = "Принтер: ошибка печати.";
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  // Завершаем операцию
  endOperationExecution("performTransponderFirmwareLoading");
}

void ClientManager::performTransponderFirmwareReloading(
    TransponderInfoModel* model,
    const QString& pan) {
  QString ucid;
  QMap<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);

  // Начинаем операцию
  if (!startOperationExecution("performTransponderFirmwareReloading")) {
    return;
  }

  emit logging("Разблокирование памяти транспондера. ");
  emit unlockDevice_signal();
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при разблокировании памяти транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  emit getUcid_signal(&ucid);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при считывании UCID с транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("ucid", ucid);
  requestParameters.insert("pan", pan);
  emit logging("Запрос перевыпуска транспондера. ");
  emit requestTransponderRerelease_signal(&requestParameters, &firmware);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при перевыпуске транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  emit loadFirmware_signal(&firmware);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  emit logging("Отправка запроса на подтверждение перевыпуска транспондера. ");
  emit requestTransponderRereleaseConfirm_signal(&requestParameters,
                                                 responseParameters);
  WaitingLoop->exec();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  //  firmware.remove();

  // Завершаем операцию
  endOperationExecution("performTransponderFirmwareReloading");
}

void ClientManager::performLocalFirmwareLoading(const QString& path,
                                                bool unlockOption) {
  // Начинаем операцию
  if (!startOperationExecution("performLocalFirmwareLoading")) {
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
  endOperationExecution("performLocalFirmwareLoading");
}

void ClientManager::performFirmwareReading() {
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

void ClientManager::performFirmwareErasing() {
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

void ClientManager::performDataReading() {
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

void ClientManager::performDataLoading(const QString& path) {
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

void ClientManager::performDeviceUnlock() {
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

void ClientManager::performDeviceLock() {
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

void ClientManager::applySettings() {
  emit logging("Применение новых настроек. ");
  loadSettings();

  emit applySettings_signal();
  Printer->applySetting();
}

void ClientManager::loadSettings() {}

void ClientManager::createProgrammerInstance() {
  // Создаем поток для программатора
  ProgrammerThread = new QThread(this);

  // И интерфейс программатора
  Programmer = new JLinkExeProgrammer(nullptr);
  Programmer->moveToThread(ProgrammerThread);

  // Подключаем логгирование к Programmer'у
  connect(Programmer, &IProgrammer::logging, this,
          &ClientManager::proxyLogging);
  // Когда поток завершит работу, объект Programmer будет удален
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ProgrammerThread, &QThread::finished, ProgrammerThread,
          &QObject::deleteLater);
  // Когда программатор завершит операцию, можно будет приступить к следующей
  connect(Programmer, &IProgrammer::operationFinished, this,
          &ClientManager::on_ProgrammerOperationFinished_slot);

  // Подключаем функционал
  connect(this, &ClientManager::getUcid_signal, Programmer,
          &IProgrammer::getUcid);
  connect(this, &ClientManager::loadFirmware_signal, Programmer,
          &IProgrammer::loadFirmware);
  connect(this, &ClientManager::loadFirmwareWithUnlock_signal, Programmer,
          &IProgrammer::loadFirmwareWithUnlock);
  connect(this, &ClientManager::readFirmware_signal, Programmer,
          &IProgrammer::readFirmware);
  connect(this, &ClientManager::eraseFirmware_signal, Programmer,
          &IProgrammer::eraseFirmware);
  connect(this, &ClientManager::loadData_signal, Programmer,
          &IProgrammer::loadData);
  connect(this, &ClientManager::readData_signal, Programmer,
          &IProgrammer::readData);
  connect(this, &ClientManager::unlockDevice_signal, Programmer,
          &IProgrammer::unlockDevice);
  connect(this, &ClientManager::lockDevice_signal, Programmer,
          &IProgrammer::lockDevice);
  connect(this, &ClientManager::applySettings_signal, Programmer,
          &IProgrammer::applySettings);

  // Запускаем поток програматора
  ProgrammerThread->start();
}

void ClientManager::createClientInstance() {
  // Создаем поток для программатора
  ClientThread = new QThread(this);

  // И интерфейс программатора
  Client = new PersoClient(nullptr);
  Client->moveToThread(ClientThread);

  // Подключаем логгирование к клиенту
  connect(Client, &PersoClient::logging, this, &ClientManager::proxyLogging);
  // Когда поток завершит работу, клиент будет удален
  connect(ClientThread, &QThread::finished, Programmer, &QObject::deleteLater);
  // Когда поток завершит работу, он будет удален
  connect(ClientThread, &QThread::finished, ClientThread,
          &QObject::deleteLater);
  // Когда клиент завершит операцию, можно будет приступить к следующей
  connect(Client, &PersoClient::operationFinished, this,
          &ClientManager::on_ClientOperationFinished_slot);

  // Подключаем функционал

  connect(this, &ClientManager::connectToPersoServer_signal, Client,
          &PersoClient::connectToPersoServer);
  connect(this, &ClientManager::disconnectFromPersoServer_signal, Client,
          &PersoClient::disconnectFromPersoServer);
  connect(this, &ClientManager::requestEcho_signal, Client,
          &PersoClient::requestEcho);
  connect(this, &ClientManager::requestAuthorize_signal, Client,
          &PersoClient::requestAuthorize);
  connect(this, &ClientManager::requestTransponderRelease_signal, Client,
          &PersoClient::requestTransponderRelease);
  connect(this, &ClientManager::requestTransponderReleaseConfirm_signal, Client,
          &PersoClient::requestTransponderReleaseConfirm);
  connect(this, &ClientManager::requestTransponderRerelease_signal, Client,
          &PersoClient::requestTransponderRerelease);
  connect(this, &ClientManager::requestTransponderRereleaseConfirm_signal,
          Client, &PersoClient::requestTransponderRereleaseConfirm);
  connect(this, &ClientManager::applySettings_signal, Client,
          &PersoClient::applySettings);

  // Запускаем поток клиента
  ClientThread->start();
}

void ClientManager::createPrinterInstance() {
  Printer = new TE310Printer(this);
  connect(Printer, &IStickerPrinter::logging, this,
          &ClientManager::proxyLogging);
}

void ClientManager::createWaitingLoop() {
  WaitingLoop = new QEventLoop(this);
  connect(this, &ClientManager::waitingEnd, WaitingLoop, &QEventLoop::quit);
}

void ClientManager::createTimers() {
  // Таймер, отслеживающий длительность выполняющихся операций
  ODTimer = new QTimer(this);
  ODTimer->setInterval(FIRMWARE_MANAGER_OPERATION_MAX_DURATION);
  connect(ODTimer, &QTimer::timeout, this,
          &ClientManager::on_ODTimerTimeout_slot);
  connect(ODTimer, &QTimer::timeout, ODTimer, &QTimer::stop);
  connect(this, &ClientManager::operationPerformingFinished, ODTimer,
          &QTimer::stop);

  // Таймер для измерения длительности операции
  ODMeter = new QElapsedTimer();
}

void ClientManager::setupODQTimer(uint32_t msecs) {
  // Таймер, отслеживающий квант длительности операции
  ODQTimer = new QTimer(this);
  ODQTimer->setInterval(msecs);

  connect(ODQTimer, &QTimer::timeout, this,
          &ClientManager::on_ODQTimerTimeout_slot);
  connect(this, &ClientManager::operationPerformingFinished, ODQTimer,
          &QTimer::stop);
}

bool ClientManager::startOperationExecution(const QString& operationName) {
  // Проверяем готовность к выполнению операции
  if (CurrentState != Ready) {
    return false;
  }

  // Переходим в состояние ожидания конца обработки
  CurrentState = WaitingExecution;

  // Настраиваем и запускаем таймер для измерения квантов времени
  QSettings settings;
  uint64_t operationDuration =
      settings
          .value(QString("ClientManager/Operations/") + operationName +
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

void ClientManager::endOperationExecution(const QString& operationName) {
  QSettings settings;

  // Сигнал о завершении текущей операции
  emit operationPerformingFinished();

  // Оповещаем пользователя о результатах
  if (CurrentState == Completed) {
    // Измеряем и сохраняем длительность операции
    uint64_t duration = ODMeter->elapsed();
    emit logging(
        QString("Длительность операции: %1.").arg(QString::number(duration)));
    settings.setValue(QString("ClientManager/Operations/") + operationName +
                          QString("/Duration"),
                      QVariant::fromValue(duration));

    emit notifyUser(NotificationText);
  } else {
    emit notifyUserAboutError(NotificationText);
  }

  // Готовы к следующей операции
  CurrentState = Ready;
}

void ClientManager::deleteClientInstance() {
  if (!ClientThread->isRunning())
    return;

  ClientThread->quit();
  ClientThread->wait();
  delete ClientThread;
  delete Client;
}

void ClientManager::deleteProgrammerInstance() {
  if (!ProgrammerThread->isRunning())
    return;

  ProgrammerThread->quit();
  ProgrammerThread->wait();
  delete ProgrammerThread;
  delete Programmer;
}

void ClientManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "IProgrammer")
    emit logging("Programmer - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else if (sender()->objectName() == "IStickerPrinter")
    emit logging("Printer - " + log);
  else
    emit logging("Unknown - " + log);
}

void ClientManager::on_ProgrammerOperationFinished_slot(
    IProgrammer::ExecutionStatus status) {
  switch (status) {
    case IProgrammer::NotExecuted:
      CurrentState = Failed;
      NotificationText = "Программатор: операция не была запущена. ";
      emit break;
    case IProgrammer::RequestParameterError:
      CurrentState = Failed;
      NotificationText = "Программатор: некорректный файл прошивки. ";
      break;
    case IProgrammer::DataFileError:
      CurrentState = Failed;
      NotificationText =
          "Программатор: некорректный файл данных для загрузки. ";
      break;
    case IProgrammer::ProgrammatorError:
      CurrentState = Failed;
      NotificationText =
          "Программатор: получена ошибка при выполнении операции. ";
      break;
    case IProgrammer::CompletedSuccessfully:
      CurrentState = Completed;
      NotificationText = "Операция успешно выполнена. ";
      break;
  }

  // Завершаем цикл ожидания
  emit waitingEnd();
}

void ClientManager::on_ClientOperationFinished_slot(
    PersoClient::ExecutionStatus status) {
  switch (status) {
    case PersoClient::NotExecuted:
      CurrentState = Failed;
      NotificationText = "Клиент: операция не была запущена. ";
      emit break;
    case PersoClient::RequestParameterError:
      CurrentState = Failed;
      NotificationText = "Клиент: некорректный файл прошивки. ";
      break;
    case PersoClient::ServerConnectionError:
      CurrentState = Failed;
      NotificationText = "Клиент: не удалось подключиться к серверу. ";
      break;
    case PersoClient::ServerNotResponding:
      CurrentState = Failed;
      NotificationText = "Клиент: сервер не отвечает. ";
      break;
    case PersoClient::ServerConnectionTerminated:
      CurrentState = Failed;
      NotificationText = "Клиент: соединение с сервером прервалось. ";
      break;
    case PersoClient::AuthorizationNotExist:
      CurrentState = Failed;
      NotificationText = "Клиент: неверный логин или пароль. ";
      break;
    case PersoClient::AuthorizationAccessDenied:
      CurrentState = Failed;
      NotificationText = "Клиент: доступ запрещен. ";
      break;
    case PersoClient::AuthorizationNotActive:
      CurrentState = Failed;
      NotificationText = "Клиент: производственная линия не активна. ";
      break;
    case PersoClient::ResponseSyntaxError:
      CurrentState = Failed;
      NotificationText = "Клиент: получен некорректный ответ от сервера. ";
      break;
    case PersoClient::ServerError:
      CurrentState = Failed;
      NotificationText = "Клиент: получена серверная ошибка. ";
      break;
    case PersoClient::UnknownError:
      CurrentState = Failed;
      NotificationText = "Клиент: неизвестная ошибка. ";
      break;
    case PersoClient::CompletedSuccessfully:
      CurrentState = Completed;
      NotificationText = "Операция успешно выполнена. ";
      break;
  }

  // Завершаем цикл ожидания
  emit waitingEnd();
}

void ClientManager::on_ODTimerTimeout_slot() {
  emit logging("Операция выполняется слишком долго. Сброс. ");
  emit notifyUserAboutError("Операция выполняется слишком долго. Сброс. ");

  //  deleteClientInstance();
  //  createClientInstance();

  //  deleteProgrammerInstance();
  //  createProgrammerInstance();
}

void ClientManager::on_ODQTimerTimeout_slot() {
  emit operationStepPerfomed();
}
