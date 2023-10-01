#include "perso_client.h"

PersoClient::PersoClient(QObject *parent) : QObject(parent)
{
  setObjectName("PersoClient");
  applySettings();

  Socket = new QTcpSocket(this);
  connect(Socket, &QTcpSocket::connected, this,
          &PersoClient::on_SocketConnected_slot);
  connect(Socket, &QTcpSocket::disconnected, this,
          &PersoClient::on_SocketDisconnected_slot);
  connect(Socket, &QTcpSocket::readyRead, this,
          &PersoClient::on_SocketReadyRead_slot);
  connect(Socket,
          QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this,
          &PersoClient::on_SocketError_slot);

  WaitTimer = new QTimer(this);
  WaitTimer->setInterval(PERSO_SERVER_CONNECTION_WAITING_TIME);
  connect(WaitTimer, &QTimer::timeout, this,
          &PersoClient::on_WaitTimerTimeout_slot);
  connect(WaitTimer, &QTimer::timeout, WaitTimer, &QTimer::stop);
  connect(Socket, &QTcpSocket::readyRead, WaitTimer, &QTimer::stop);
  connect(Socket, &QTcpSocket::connected, WaitTimer, &QTimer::stop);
  connect(Socket, &QTcpSocket::disconnected, WaitTimer, &QTimer::stop);
  connect(Socket,
          QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
          WaitTimer, &QTimer::stop);

  WaitingLoop = new QEventLoop(this);
  connect(Socket, &QTcpSocket::connected, WaitingLoop, &QEventLoop::quit);
  connect(Socket, &QTcpSocket::disconnected, WaitingLoop, &QEventLoop::quit);
  connect(Socket,
          QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
          WaitingLoop, &QEventLoop::quit);
  connect(WaitTimer, &QTimer::timeout, WaitingLoop, &QEventLoop::quit);
  connect(this, &PersoClient::stopResponseWaiting, WaitingLoop,
          &QEventLoop::quit);

  // Команды еще не выполнялись
  ReturnStatus = NotExecuted;

  // Готовы к выполнению команд
  CurrentState = Ready;

  // Регистрация в мета-объектной системе Qt пользовательские типы
  qRegisterMetaType<ExecutionStatus>("ExecutionStatus");
}

PersoClient::~PersoClient() {
  if (Socket->isOpen())
    Socket->disconnectFromHost();
}

void PersoClient::applySettings() {
  QSettings settings;
  emit logging("Применение новых настроек. ");

  PersoServerAddress =
      settings.value("Personalization/ServerIpAddress").toString();
  PersoServerPort = settings.value("Personalization/ServerPort").toInt();
}

void PersoClient::connectToPersoServer() {
  CurrentState = Ready;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
  } else {
    CurrentState = ConnectedToServer;
    emit operationFinished(CompletedSuccessfully);
  }
}

void PersoClient::disconnectFromPersoServer() {
  if (Socket->isOpen()) {
    Socket->disconnectFromHost();
    emit logging("Отключение от сервера персонализации. ");
  } else {
    emit logging("Подключение не было установлено. ");
  }

  CurrentState = DisconnectedFromServer;
  emit operationFinished(CompletedSuccessfully);
}

void PersoClient::requestEcho() {
  CurrentState = Ready;

  // Создаем запрос
  createEchoRequest();
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }

  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processEchoResponse();

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestAuthorize(
    const QMap<QString, QString>* requestAttributes) {
  CurrentState = Ready;

  // Проверка на существование
  if (!requestAttributes) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    emit operationFinished(RequestParameterError);
    return;
  }

  // Создаем запрос
  createAuthorizationRequest(requestAttributes);
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }
  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processAuthorizationResponse();

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestTransponderRelease(
    const QMap<QString, QString>* requestAttributes,
    QFile* firmware) {
  CurrentState = Ready;

  // Проверка на существование
  if ((!requestAttributes) || (!firmware)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    emit operationFinished(RequestParameterError);
    return;
  }

  // Создаем запрос
  createTransponderRelease(requestAttributes);
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }

  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processTransponderReleaseResponse(firmware);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestTransponderReleaseConfirm(
    const QMap<QString, QString>* requestAttributes,
    QMap<QString, QString>* responseAttributes) {
  CurrentState = Ready;

  // Проверка на существование
  if ((!requestAttributes) || (!responseAttributes)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    emit operationFinished(RequestParameterError);
    return;
  }

  // Создаем запрос
  createTransponderReleaseConfirm(requestAttributes);
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }

  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processTransponderReleaseConfirmResponse(responseAttributes);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestTransponderRerelease(
    const QMap<QString, QString>* requestAttributes,
    QFile* firmware) {
  CurrentState = Ready;

  // Проверка на существование
  if ((!firmware) || (requestAttributes->isEmpty())) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    emit operationFinished(RequestParameterError);
    return;
  }

  // Создаем запрос
  createTransponderRelease(requestAttributes);
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }

  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processTransponderReleaseResponse(firmware);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestTransponderRereleaseConfirm(
    const QMap<QString, QString>* requestAttributes,
    QMap<QString, QString>* responseAttributes) {
  CurrentState = Ready;

  // Проверка на существование
  if ((!requestAttributes) || (!responseAttributes)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    emit operationFinished(RequestParameterError);
    return;
  }

  // Создаем запрос
  createTransponderReleaseConfirm(requestAttributes);
  CurrentState = RequestCreated;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    CurrentState = DisconnectedFromServer;
    emit operationFinished(ServerConnectionError);
    return;
  }

  CurrentState = ConnectedToServer;

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();
  CurrentState = DisconnectedFromServer;

  // Обрабатываем полученный ответ
  processTransponderRereleaseConfirmResponse(responseAttributes);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

bool PersoClient::processingServerConnection() {
  emit logging("Подключение к серверу персонализации. ");
  // Подключаемся к серверу персонализации
  Socket->connectToHost(PersoServerAddress, PersoServerPort);

  // Ожидаем подключения или отказа
  emit logging("Ожидание ответа от сервера. ");
  CurrentState = WaitingConnectionWithServer;
  WaitTimer->start();
  WaitingLoop->exec();

  // Если соединение не удалось установить
  if (!Socket->isOpen()) {
    emit logging(
        "Не удалось установить соединение с сервером персонализации. Сброс. ");
    return false;
  }

  return true;
}

void PersoClient::processingDataBlock() {
  QJsonParseError status;
  QJsonDocument responseDocument =
      QJsonDocument::fromJson(ReceivedDataBlock, &status);

  // Если пришел некорректный JSON
  if (status.error != QJsonParseError::NoError) {
    emit logging("Ошибка парсинга JSON команды. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  emit logging(QString("Размер ответа на команду: %1. Содержание ответа: %2. ")
                   .arg(QString::number(responseDocument.toJson().size()),
                        QString(responseDocument.toJson())));

  // Выделяем список пар ключ-значение из JSON-файла
  CurrentResponse = responseDocument.object();
}

void PersoClient::createTransmittedDataBlock(void) {
  QJsonDocument requestDocument(CurrentCommand);
  emit logging(QString("Содержание команды: %1 ")
                   .arg(QString(requestDocument.toJson())));
  emit logging("Формирование блока данных для команды. ");

  // Инициализируем блок данных и сериализатор
  TransmittedDataBlock.clear();
  QDataStream serializator(&TransmittedDataBlock, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_5_12);

  // Формируем единый блок данных для отправки
  serializator << uint32_t(0) << requestDocument.toJson();
  serializator.device()->seek(0);
  serializator << uint32_t(TransmittedDataBlock.size() - sizeof(uint32_t));
}

void PersoClient::transmitDataBlock() {
  // Ответный блок данных еще не получен
  ReceivedDataBlockSize = 0;

  // Если размер блок не превышает максимального размера данных для единоразовой
  // передачи
  if (TransmittedDataBlock.size() < ONETIME_TRANSMIT_DATA_SIZE) {
    // Отправляем блок данных
    Socket->write(TransmittedDataBlock);
  } else {
    // В противном случае дробим блок данных на части и последовательно
    // отправляем
    for (int32_t i = 0; i < TransmittedDataBlock.size();
         i += ONETIME_TRANSMIT_DATA_SIZE) {
      Socket->write(TransmittedDataBlock.mid(i, ONETIME_TRANSMIT_DATA_SIZE));
    }
  }
  CurrentState = WaitingResponse;

  // Ожидаем ответ
  WaitTimer->start();
  WaitingLoop->exec();
  CurrentState = ResponseReceived;
}

void PersoClient::createEchoRequest(void) {
  emit logging("Формирование команды Echo. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "Echo";

  // Тело команды
  CurrentCommand["data"] = "Test";

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createAuthorizationRequest(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды Authorization. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "Authorization";

  // Тело команды
  CurrentCommand["login"] = requestAttributes->value("login");
  CurrentCommand["password"] = requestAttributes->value("password");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createTransponderRelease(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRelease. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "TransponderRelease";

  // Тело команды
  CurrentCommand["login"] = requestAttributes->value("login");
  CurrentCommand["password"] = requestAttributes->value("password");
  CurrentCommand["ucid"] = requestAttributes->value("ucid");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createTransponderReleaseConfirm(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderReleaseConfirm. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "TransponderReleaseConfirm";

  // Тело команды
  CurrentCommand["login"] = requestAttributes->value("login");
  CurrentCommand["password"] = requestAttributes->value("password");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createTransponderRerelease(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRerelease. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "TransponderRerelease";

  // Тело команды
  CurrentCommand["login"] = requestAttributes->value("login");
  CurrentCommand["password"] = requestAttributes->value("password");
  CurrentCommand["ucid"] = requestAttributes->value("ucid");
  CurrentCommand["pan"] = requestAttributes->value("pan");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createTransponderRereleaseConfirm(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRereleaseConfirm. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "TransponderRereleaseConfirm";

  // Тело команды
  CurrentCommand["login"] = requestAttributes->value("login");
  CurrentCommand["password"] = requestAttributes->value("password");
  CurrentCommand["ucid"] = requestAttributes->value("ucid");
  CurrentCommand["pan"] = requestAttributes->value("pan");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::processEchoResponse(void) {
  emit logging("Обработка ответа на команду Echo. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  if (CurrentResponse.value("data").isUndefined() ||
      CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() != "Echo")) {
    emit logging("Обнаружена синтаксическая ошибка в ответе Echo. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  emit logging("Команда Echo успешно выполнена. ");
  ReturnStatus = CompletedSuccessfully;
}

void PersoClient::processAuthorizationResponse(void) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() != "Authorization") ||
      CurrentResponse.value("access").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе Authorization. Сброс. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  if (CurrentResponse.value("access").toString() == "Allowed") {
    ReturnStatus = CompletedSuccessfully;
  } else if (CurrentResponse.value("access").toString() == "NotExist") {
    ReturnStatus = AuthorizationNotExist;
  } else if (CurrentResponse.value("access").toString() == "NotActive") {
    ReturnStatus = AuthorizationNotActive;
  } else {
    ReturnStatus = AuthorizationAccessDenied;
  }

  emit logging("Команда Authorization успешно выполнена. ");
}

void PersoClient::processTransponderReleaseResponse(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() !=
       "TransponderRelease") ||
      CurrentResponse.value("firmware").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderRelease. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ReturnStatus = ServerError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(QByteArray::fromBase64(
      CurrentResponse.value("firmware").toString().toUtf8()));
  firmware->close();

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderRelease успешно выполнена. ");
}

void PersoClient::processTransponderReleaseConfirmResponse(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderReleaseConfirm. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() ==
       "TransponderReleaseConfirm") ||
      CurrentResponse.value("sn").isUndefined() ||
      CurrentResponse.value("pan").isUndefined() ||
      CurrentResponse.value("box_id").isUndefined() ||
      CurrentResponse.value("pallet_id").isUndefined() ||
      CurrentResponse.value("order_id").isUndefined() ||
      CurrentResponse.value("issuer_name").isUndefined() ||
      CurrentResponse.value("manufacturer_id").isUndefined() ||
      CurrentResponse.value("battery_insertation_date").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderReleaseConfirm. "
        "Сброс. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  responseAttributes->insert("sn", CurrentResponse.value("sn").toString());
  responseAttributes->insert("pan", CurrentResponse.value("pan").toString());
  responseAttributes->insert("box_id",
                             CurrentResponse.value("box_id").toString());
  responseAttributes->insert("pallet_id",
                             CurrentResponse.value("pallet_id").toString());
  responseAttributes->insert("order_id",
                             CurrentResponse.value("order_id").toString());
  responseAttributes->insert("issuer_name",
                             CurrentResponse.value("issuer_name").toString());
  responseAttributes->insert(
      "manufacturer_id", CurrentResponse.value("manufacturer_id").toString());
  responseAttributes->insert(
      "battery_insertation_date",
      CurrentResponse.value("battery_insertation_date").toString());

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderReleaseConfirm успешно выполнена. ");
}

void PersoClient::processTransponderRereleaseResponse(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRerelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() !=
       "TransponderRerelease") ||
      CurrentResponse.value("firmware").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderRerelease: "
        "отсутствует файл прошивки. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(QByteArray::fromBase64(
      CurrentResponse.value("firmware").toString().toUtf8()));
  firmware->close();

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderRerelease успешно выполнена. ");
}

void PersoClient::processTransponderRereleaseConfirmResponse(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderRereleaseConfirm. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ReturnStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() ==
       "TransponderReleaseConfirm") ||
      CurrentResponse.value("sn").isUndefined() ||
      CurrentResponse.value("pan").isUndefined() ||
      CurrentResponse.value("box_id").isUndefined() ||
      CurrentResponse.value("pallet_id").isUndefined() ||
      CurrentResponse.value("order_id").isUndefined() ||
      CurrentResponse.value("issuer_name").isUndefined() ||
      CurrentResponse.value("manufacturer_id").isUndefined() ||
      CurrentResponse.value("battery_insertation_date").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderReleaseConfirm. "
        "Сброс. ");
    ReturnStatus = ResponseSyntaxError;
    return;
  }

  responseAttributes->insert("sn", CurrentResponse.value("sn").toString());
  responseAttributes->insert("pan", CurrentResponse.value("pan").toString());
  responseAttributes->insert("box_id",
                             CurrentResponse.value("box_id").toString());
  responseAttributes->insert("pallet_id",
                             CurrentResponse.value("pallet_id").toString());
  responseAttributes->insert("order_id",
                             CurrentResponse.value("order_id").toString());
  responseAttributes->insert("issuer_name",
                             CurrentResponse.value("issuer_name").toString());
  responseAttributes->insert(
      "manufacturer_id", CurrentResponse.value("manufacturer_id").toString());
  responseAttributes->insert(
      "battery_insertation_date",
      CurrentResponse.value("battery_insertation_date").toString());

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderRereleaseConfirm успешно выполнена. ");
}

void PersoClient::on_SocketDisconnected_slot() {
  emit logging("Соединение с сервером персонализации отключено. ");
}

void PersoClient::on_SocketConnected_slot() {
  emit logging("Соединение с сервером персонализации установлено. ");
}

void PersoClient::on_SocketReadyRead_slot() {
  QDataStream deserializator(Socket);  // Дессериализатор
  deserializator.setVersion(
      QDataStream::Qt_5_12);  // Настраиваем версию десериализатора

  // Если блок данных еще не начал формироваться
  if (ReceivedDataBlockSize == 0) {
    // Если размер поступивших байт меньше размера поля с размером байт, то
    // блок поступившие данные отбрасываются
    if (Socket->bytesAvailable() < static_cast<int64_t>(sizeof(uint32_t))) {
      emit logging(
          "Размер полученных данных слишком мал. Ожидается прием следующих "
          "частей. ");
      // Перезапускаем таймер ожидания для следующих частей
      WaitTimer->start();
      return;
    }
    // Сохраняем размер блока данных
    deserializator >> ReceivedDataBlockSize;

    emit logging(QString("Размер ожидаемого блока данных: %1.")
                     .arg(QString::number(ReceivedDataBlockSize)));

    // Если размер блока данных слишком большой, то весь блок отбрасывается
    if (ReceivedDataBlockSize > DATA_BLOCK_MAX_SIZE) {
      emit logging("Размер блока данных слишком большой. Сброс. ");
      // Останавливаем цикл ожидания
      emit stopResponseWaiting();
      ReceivedDataBlockSize = 0;
    }
  }

  emit logging(QString("Размер принятых данных: %1. ")
                   .arg(QString::number(Socket->bytesAvailable())));

  // Дожидаемся пока весь блок данных придет целиком
  if (Socket->bytesAvailable() < ReceivedDataBlockSize) {
    emit logging("Блок получен не целиком. Ожидается прием следующих частей. ");
    // Перезапускаем таймер ожидания для следующих частей
    WaitTimer->start();
    return;
  }

  // Если блок был получен целиком, то осуществляем его дессериализацию
  deserializator >> ReceivedDataBlock;

  // Осуществляем первичную обработку полученных данных
  processingDataBlock();

  // Останавливаем цикл ожидания
  emit stopResponseWaiting();
}

void PersoClient::on_SocketError_slot(
    QAbstractSocket::SocketError socketError) {
  ReturnStatus = ServerConnectionTerminated;
  CurrentState = DisconnectedFromServer;

  emit logging(QString("Ошибка сети: Код: %1. Описание: %2.")
                   .arg(QString::number(socketError), Socket->errorString()));
  Socket->close();

  emit operationFinished(ReturnStatus);
}

void PersoClient::on_WaitTimerTimeout_slot() {
  if (CurrentState == WaitingConnectionWithServer) {
    ReturnStatus = ServerConnectionError;
  } else if (CurrentState == WaitingResponse) {
    ReturnStatus = ServerNotResponding;
  } else {
    ReturnStatus = UnknownError;
  }

  CurrentState = DisconnectedFromServer;

  emit logging("Время ожидания вышло. ");
  Socket->abort();

  emit operationFinished(ReturnStatus);
}
