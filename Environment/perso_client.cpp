#include "perso_client.h"

PersoClient::PersoClient(QObject *parent) : QObject(parent)
{
  setObjectName("PersoClient");
  loadSettings();

  // Создаем сокет
  createSocket();

  // Создаем таймеры
  createTimers();

  // Готовы к выполнению команд
  CurrentState = Ready;
}

PersoClient::~PersoClient() {
  if (Socket->isOpen())
    Socket->disconnectFromHost();
}

PersoClient::ReturnStatus PersoClient::connectToServer() {
  CurrentState = Ready;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  return Completed;
}

PersoClient::ReturnStatus PersoClient::disconnectFromServer() {
  if (Socket->isOpen()) {
    Socket->disconnectFromHost();
    emit logging("Отключение от сервера персонализации. ");
  } else {
    emit logging("Подключение не было установлено. ");
  }

  return Completed;
}

PersoClient::ReturnStatus PersoClient::requestEcho() {
  // Создаем запрос
  CurrentState = CreatingRequest;
  createEcho();

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processEcho();

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

PersoClient::ReturnStatus PersoClient::requestAuthorize(
    const QMap<QString, QString>* requestAttributes) {
  // Создаем запрос
  CurrentState = CreatingRequest;

  // Проверка на существование
  if (!requestAttributes) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    return RequestParameterError;
  }
  createAuthorization(requestAttributes);

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processAuthorization();

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

PersoClient::ReturnStatus PersoClient::requestTransponderRelease(
    const QMap<QString, QString>* requestAttributes,
    QFile* firmware) {
  // Создаем запрос
  CurrentState = CreatingRequest;

  // Проверка на существование
  if ((!requestAttributes) || (!firmware)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    return RequestParameterError;
  }
  createTransponderRelease(requestAttributes);

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processTransponderRelease(firmware);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

PersoClient::ReturnStatus PersoClient::requestTransponderReleaseConfirm(
    const QMap<QString, QString>* requestAttributes,
    QMap<QString, QString>* responseAttributes) {
  // Создаем запрос
  CurrentState = CreatingRequest;

  // Проверка на существование
  if ((!requestAttributes) || (!responseAttributes)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    return RequestParameterError;
  }
  createTransponderReleaseConfirm(requestAttributes);

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processTransponderReleaseConfirm(responseAttributes);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

PersoClient::ReturnStatus PersoClient::requestTransponderRerelease(
    const QMap<QString, QString>* requestAttributes,
    QFile* firmware) {
  // Создаем запрос
  CurrentState = CreatingRequest;

  // Проверка на существование
  if ((!requestAttributes) || (!firmware)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    return RequestParameterError;
  }
  createTransponderRerelease(requestAttributes);

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processTransponderRerelease(firmware);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

PersoClient::ReturnStatus PersoClient::requestTransponderRereleaseConfirm(
    const QMap<QString, QString>* requestAttributes,
    QMap<QString, QString>* responseAttributes) {
  // Создаем запрос
  CurrentState = CreatingRequest;

  // Проверка на существование
  if ((!requestAttributes) || (!responseAttributes)) {
    emit logging("Получены не корректные параметры запроса. Сброс.");
    return RequestParameterError;
  }
  createTransponderRereleaseConfirm(requestAttributes);

  // Подключаемся к серверу персонализации
  CurrentState = WaitingServerConnection;
  if (!processingServerConnection()) {
    return ServerConnectionError;
  }

  // Отправляем сформированный блок данных
  CurrentState = WaitingResponse;
  transmitDataBlock();

  // Если сервер не отвечает
  if (TimeoutIndicator) {
    return ServerNotResponding;
  }

  // Отключаемся от сервера
  Socket->close();

  // Обрабатываем полученный ответ
  CurrentState = ProcessingResponse;
  processTransponderRereleaseConfirm(responseAttributes);

  // Очищаем команду и ответ на нее
  CurrentCommand = QJsonObject();
  CurrentResponse = QJsonObject();

  // Замыкаем машину состояний
  CurrentState = Ready;

  // Возвращаем статус выполнения команды
  return ProcessingStatus;
}

void PersoClient::applySettings() {
  emit logging("Применение новых настроек. ");
  loadSettings();
}

void PersoClient::sendLog(const QString& log) {
  if (LogEnable) {
    emit logging(log);
  }
}

/*
 * Приватные  методы
 */

void PersoClient::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/log_enable").toBool();
  ExtendedLoggingEnable = settings.value("log_system/extended_enable").toBool();

  PersoServerAddress = settings.value("perso_client/server_ip").toString();
  PersoServerPort = settings.value("perso_client/server_port").toInt();
}

void PersoClient::createTimers() {
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
}

void PersoClient::createSocket() {
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
}

bool PersoClient::processingServerConnection() {
  emit logging("Подключение к серверу персонализации. ");
  // Подключаемся к серверу персонализации
  Socket->connectToHost(PersoServerAddress, PersoServerPort);

  // Ожидаем подключения или отказа
  emit logging("Ожидание ответа от сервера. ");
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
    return;
  }

  emit logging(QString("Размер ответа на команду: %1.")
                   .arg(QString::number(responseDocument.toJson().size())));
  if (ExtendedLoggingEnable == true) {
    emit logging(QString("Содержание ответа: %1 ")
                     .arg(QString(responseDocument.toJson())));
  }

  // Выделяем список пар ключ-значение из JSON-файла
  CurrentResponse = responseDocument.object();
}

void PersoClient::createTransmittedDataBlock(void) {
  QJsonDocument requestDocument(CurrentCommand);

  emit logging(QString("Размер команды: %1 ")
                   .arg(QString::number(requestDocument.toJson().size())));
  if (ExtendedLoggingEnable == true) {
    emit logging(QString("Содержание команды: %1 ")
                     .arg(QString(requestDocument.toJson())));
  }
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

  // Ожидаем ответ
  WaitTimer->start();
  WaitingLoop->exec();
}

void PersoClient::createEcho(void) {
  emit logging("Формирование команды Echo. ");

  // Заголовок команды
  CurrentCommand["command_name"] = "Echo";

  // Тело команды
  CurrentCommand["data"] = "Test";

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::createAuthorization(
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
  CurrentCommand["ucid"] = requestAttributes->value("ucid");

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
  CurrentCommand["pan"] = requestAttributes->value("pan");
  CurrentCommand["ucid"] = requestAttributes->value("ucid");

  // Создаем блок данных для команды
  createTransmittedDataBlock();
}

void PersoClient::processEcho(void) {
  emit logging("Обработка ответа на команду Echo. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
    return;
  }

  if (CurrentResponse.value("data").isUndefined() ||
      CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() != "Echo")) {
    emit logging("Обнаружена синтаксическая ошибка в ответе Echo. ");
    ProcessingStatus = ResponseSyntaxError;
    return;
  }

  emit logging("Команда Echo успешно выполнена. ");
  ProcessingStatus = Completed;
}

void PersoClient::processAuthorization(void) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() != "Authorization") ||
      CurrentResponse.value("access").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе Authorization. Сброс. ");
    ProcessingStatus = ResponseSyntaxError;
    return;
  }

  if (CurrentResponse.value("access").toString() == "Allowed") {
    ProcessingStatus = Completed;
  } else if (CurrentResponse.value("access").toString() == "NotExist") {
    ProcessingStatus = AuthorizationNotExist;
  } else if (CurrentResponse.value("access").toString() == "NotActive") {
    ProcessingStatus = AuthorizationNotActive;
  } else {
    ProcessingStatus = AuthorizationAccessDenied;
  }

  emit logging("Команда Authorization успешно выполнена. ");
}

void PersoClient::processTransponderRelease(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() !=
       "TransponderRelease") ||
      CurrentResponse.value("firmware").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderRelease. ");
    ProcessingStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ProcessingStatus = ServerError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(QByteArray::fromBase64(
      CurrentResponse.value("firmware").toString().toUtf8()));
  firmware->close();

  ProcessingStatus = Completed;
  emit logging("Команда TransponderRelease успешно выполнена. ");
}

void PersoClient::processTransponderReleaseConfirm(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderReleaseConfirm. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() !=
       "TransponderReleaseConfirm") ||
      CurrentResponse.value("sn").isUndefined() ||
      CurrentResponse.value("pan").isUndefined() ||
      CurrentResponse.value("box_id").isUndefined() ||
      CurrentResponse.value("pallet_id").isUndefined() ||
      CurrentResponse.value("order_id").isUndefined() ||
      CurrentResponse.value("issuer_name").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderReleaseConfirm. "
        "Сброс. ");
    ProcessingStatus = ResponseSyntaxError;
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

  ProcessingStatus = Completed;
  emit logging("Команда TransponderReleaseConfirm успешно выполнена. ");
}

void PersoClient::processTransponderRerelease(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRerelease. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
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
    ProcessingStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ProcessingStatus = ResponseSyntaxError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(QByteArray::fromBase64(
      CurrentResponse.value("firmware").toString().toUtf8()));
  firmware->close();

  ProcessingStatus = Completed;
  emit logging("Команда TransponderRerelease успешно выполнена. ");
}

void PersoClient::processTransponderRereleaseConfirm(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderRereleaseConfirm. ");

  // Проверка статуса возврата
  if (CurrentResponse["return_status"] != "NoError") {
    emit logging(QString("Получена серверная ошибка: %1")
                     .arg(CurrentResponse["return_status"].toString()));
    ProcessingStatus = ServerError;
    return;
  }

  // Синтаксическая проверка
  if (CurrentResponse.value("response_name").isUndefined() ||
      (CurrentResponse.value("response_name").toString() !=
       "TransponderRereleaseConfirm") ||
      CurrentResponse.value("sn").isUndefined() ||
      CurrentResponse.value("pan").isUndefined() ||
      CurrentResponse.value("box_id").isUndefined() ||
      CurrentResponse.value("pallet_id").isUndefined() ||
      CurrentResponse.value("order_id").isUndefined() ||
      CurrentResponse.value("issuer_name").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderReleaseConfirm. "
        "Сброс. ");
    ProcessingStatus = ResponseSyntaxError;
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

  ProcessingStatus = Completed;
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
  Socket->abort();
  emit logging(QString("Ошибка сети: Код: %1. Описание: %2.")
                   .arg(QString::number(socketError), Socket->errorString()));
}

void PersoClient::on_WaitTimerTimeout_slot() {
  Socket->abort();
  TimeoutIndicator = true;
  emit logging("Время ожидания вышло. ");
}
