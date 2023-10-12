#include "client_manager.h"

ClientManager::ClientManager(QObject* parent) : QObject(parent) {
  setObjectName("ClientManager");

  // Создаем программатор
  createProgrammerInstance();

  // Создаем клиент
  createClientInstance();

  // Создаем принтер
  createStickerPrinterInstance();

  CurrentLogin = PRODUCTION_LINE_DEFAULT_LOGIN;
  CurrentPassword = PRODUCTION_LINE_DEFAULT_PASSWORD;
}

ClientManager::~ClientManager() {
}

void ClientManager::on_InsctanceThreadStarted_slot() {}

void ClientManager::performServerConnecting() {
  startOperationExecution("performServerConnecting");

  emit logging("Подключение к серверу персонализации. ");
  if (Client->connectToServer() != PersoClient::Completed) {
    emit logging("");
    endOperationExecution("performServerConnecting");
  }

  // Завершаем операцию
  endOperationExecution("performServerConnecting");
}

void ClientManager::performServerDisconnecting() {
  startOperationExecution("performServerDisconnecting");

  emit logging("Отключение от сервера персонализации. ");
  emit disconnectFromPersoServer();

  // Завершаем операцию
  endOperationExecution("performServerDisconnecting");
}

void ClientManager::performServerEcho() {
  startOperationExecution("performServerEcho");

  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  emit requestEcho();

  // Завершаем операцию
  endOperationExecution("performServerEcho");
}

void ClientManager::performServerAuthorization(
    const QMap<QString, QString>* data,
    bool& result) {
  startOperationExecution("performServerAuthorization");

  emit logging("Авторизация на сервере персонализации. ");
  emit requestAuthorize(data);

  if (CurrentState != Completed) {
    emit logging("Получена ошибка при авторизации на сервере. ");
    endOperationExecution("performServerAuthorization");
    return;
  }

  // Сохраняем данные и вызываем производственный интерфейс
  CurrentLogin = data->value("login");
  CurrentPassword = data->value("password");

  emit createProductionInterface();

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

  startOperationExecution("performTransponderFirmwareLoading");

  emit logging("Разблокирование памяти транспондера. ");
  emit unlockDevice();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при разблокировании памяти транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  emit getUcid(&ucid);
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при считывании UCID с транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  emit logging("Запрос прошивки транспондера. ");
  emit requestTransponderRelease(&requestParameters, &firmware);
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при выпуске транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  emit loadFirmware(&firmware);
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("ucid", ucid);
  emit logging(
      "Отправка запроса на подтверждение загрузки прошивки в транспондер. ");
  emit requestTransponderReleaseConfirm(&requestParameters, responseParameters);
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при подтверждении выпуска транспондера. ");
    endOperationExecution("performTransponderFirmwareLoading");
    return;
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  firmware.remove();

  emit logging("Печать стикера для транспондера.");
  if (!StickerPrinter->printTransponderSticker(responseParameters)) {
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

  startOperationExecution("performTransponderFirmwareReloading");

  emit logging("Разблокирование памяти транспондера. ");
  emit unlockDevice();
  if (CurrentState != Completed) {
    emit logging("Получена ошибка при разблокировании памяти транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  emit getUcid(&ucid);

  if (CurrentState != Completed) {
    emit logging("Получена ошибка при считывании UCID с транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  emit logging("Запрос перевыпуска транспондера. ");
  emit requestTransponderRerelease(&requestParameters, &firmware);

  if (CurrentState != Completed) {
    emit logging("Получена ошибка при перевыпуске транспондера. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  emit loadFirmware(&firmware);

  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  requestParameters.insert("ucid", ucid);
  emit logging("Отправка запроса на подтверждение перевыпуска транспондера. ");
  emit requestTransponderRereleaseConfirm(&requestParameters,
                                          responseParameters);

  if (CurrentState != Completed) {
    emit logging("Получена ошибка при загрузке прошивки в транспондер. ");
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  firmware.remove();

  emit logging("Печать стикера для транспондера.");
  if (!StickerPrinter->printTransponderSticker(responseParameters)) {
    emit logging("Получена ошибка при печати стикера для транспондера.");
    CurrentState = Failed;
    NotificationText = "Принтер: ошибка печати.";
    endOperationExecution("performTransponderFirmwareReloading");
    return;
  }

  // Завершаем операцию
  endOperationExecution("performTransponderFirmwareReloading");
}

void ClientManager::performLocalFirmwareLoading(const QString& path,
                                                bool unlockOption) {
  startOperationExecution("performLocalFirmwareLoading");

  // Создаем файл
  QFile firmware(path);

  // Вызываем соответствующую функцию
  if (unlockOption == true) {
    emit logging("Загрузка прошивки микроконтроллера. ");
    emit loadFirmware(&firmware);
  } else {
    emit logging(
        "Разблокирование памяти и загрузка прошивки микроконтроллера. ");
    emit loadFirmwareWithUnlock(&firmware);
  }

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performLocalFirmwareLoading");
}

void ClientManager::performFirmwareReading() {
  startOperationExecution("performFirmwareReading");

  emit logging("Считывание прошивки микроконтроллера. ");
  emit readFirmware();

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performFirmwareReading");
}

void ClientManager::performFirmwareErasing() {
  startOperationExecution("performFirmwareErasing");

  emit logging("Стирание прошивки микроконтроллера. ");
  emit eraseFirmware();

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performFirmwareErasing");
}

void ClientManager::performDataReading() {
  startOperationExecution("performDataReading");

  emit logging("Считывание данных в память микроконтроллера. ");
  emit readData();

  // Запуск цикла ожидания

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
  emit loadData(&dataFile);

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performDataLoading");
}

void ClientManager::performDeviceUnlock() {
  startOperationExecution("performDeviceUnlock");

  emit logging("Разблокирование памяти микроконтроллера. ");
  emit unlockDevice();

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performDeviceUnlock");
}

void ClientManager::performDeviceLock() {
  startOperationExecution("performDeviceLock");

  emit logging("Блокирование памяти микроконтроллера. ");
  emit lockDevice();

  // Запуск цикла ожидания

  // Завершаем операцию
  endOperationExecution("performDeviceLock");
}

void ClientManager::performPrintingLastTransponderSticker() {
  startOperationExecution("performPrintingLastTransponderSticker");

  emit logging("Печать последнего стикера транспондера. ");
  if (!StickerPrinter->printLastTransponderSticker()) {
    CurrentState = Failed;
    NotificationText = "Получена ошибка при печати стикера транспондера. ";
    endOperationExecution("performPrintingLastTransponderSticker");
  }

  CurrentState = Completed;
  NotificationText = "Операция успешно выполнена. ";

  // Завершаем операцию
  endOperationExecution("performPrintingLastTransponderSticker");
}

void ClientManager::performPrintingCustomTransponderSticker(
    const QMap<QString, QString>* parameters) {
  startOperationExecution("performPrintingCustomTransponderSticker");

  emit logging("Печать произвольного стикера транспондера. ");
  if (!StickerPrinter->printTransponderSticker(parameters)) {
    CurrentState = Failed;
    NotificationText = "Получена ошибка при печати стикера транспондера. ";
    endOperationExecution("performPrintingCustomTransponderSticker");
  }

  CurrentState = Completed;
  NotificationText = "Операция успешно выполнена. ";

  // Завершаем операцию
  endOperationExecution("performPrintingCustomTransponderSticker");
}

void ClientManager::performExecutingStickerPrinterCommandScript(
    const QStringList* commandScript) {
  startOperationExecution("performExecutingStickerPrinterCommandScript");

  emit logging("Выполнение командного скрипта для принтера. ");
  StickerPrinter->exec(commandScript);

  // Завершаем операцию
  endOperationExecution("performExecutingStickerPrinterCommandScript");
}

void ClientManager::applySettings() {
  emit logging("Применение новых настроек. ");
  loadSettings();

  emit applySettings();
  StickerPrinter->applySetting();
}

void ClientManager::loadSettings() {}

void ClientManager::createProgrammerInstance() {
  Programmer = new JLinkExeProgrammer(this);
  connect(Programmer, &IProgrammer::logging, this,
          &ClientManager::proxyLogging);
}

void ClientManager::createClientInstance() {
  Client = new PersoClient(this);
  connect(Client, &PersoClient::logging, this, &ClientManager::proxyLogging);

  // Заполняем таблицу соответствий статусов возврата
  ClientReturnStatusMatch.insert(PersoClient::NotExecuted,
                                 "Выполнение операции не началось.");
  ClientReturnStatusMatch.insert(PersoClient::RequestParameterError,
                                 "Не удалось обработать параметры запроса. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionError,
                                 "Не удалось подключиться к серверу. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerNotResponding,
                                 "Сервер не отвечает.");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionTerminated,
                                 "Оборвалось соединение с сервером.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotExist,
                                 "Производственная линия не существует.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationAccessDenied,
                                 "Ошибка доступа к производственной линии.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotActive,
                                 "Производственная линия не активна. ");
  ClientReturnStatusMatch.insert(PersoClient::ResponseSyntaxError,
                                 "Синтаксическая ошибка в ответе на запрос. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerError,
                                 "Серверная ошибка. ");
  ClientReturnStatusMatch.insert(PersoClient::UnknownError,
                                 "Неизвествная ошибка. ");
  ClientReturnStatusMatch.insert(PersoClient::Completed, "Выполнено. ");
}

void ClientManager::createStickerPrinterInstance() {
  StickerPrinter = new TE310Printer(this);
  connect(StickerPrinter, &IStickerPrinter::logging, this,
          &ClientManager::proxyLogging);

  // Заполняем таблицу соответствий статусов возврата
}

void ClientManager::startOperationExecution(const QString& operationName) {
  Mutex.lock();

  // Отправляем сигнал о начале выполнения длительной операции
  emit operationPerfomingStarted(operationName);
}

void ClientManager::endOperationExecution(const QString& operationName) {
  Mutex.unlock();

  // Сигнал о завершении текущей операции
  emit operationPerformingFinished(operationName);
}

void ClientManager::processClientReturnStatus(
    PersoClient::ReturnStatus status) {
  if (status != PersoClient::Completed) {
    emit notifyUserAboutError(ClientReturnStatusMatch.value(status));
    return;
  }

  emit notifyUser(ClientReturnStatusMatch.value(status));
}

void ClientManager::processProgrammerReturnStatus(
    IProgrammer::ReturnStatus status) {
  if (status != IProgrammer::Completed) {
    emit notifyUserAboutError(ProgrammerReturnStatusMatch.value(status));
    return;
  }

  emit notifyUser(ProgrammerReturnStatusMatch.value(status));
}

void ClientManager::processStickerPrintersReturnStatus(
    IStickerPrinter::ReturnStatus status) {
  if (status != IStickerPrinter::Completed) {
    emit notifyUserAboutError(StickerPrinterReturnStatusMatch.value(status));
    return;
  }

  emit notifyUser(StickerPrinterReturnStatusMatch.value(status));
}

void ClientManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "IProgrammer")
    emit logging("Programmer - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else if (sender()->objectName() == "IStickerPrinter")
    emit logging("StickerPrinter - " + log);
  else
    emit logging("Unknown - " + log);
}
