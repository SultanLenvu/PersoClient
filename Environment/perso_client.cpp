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

  // Возвращаем статус выполнения команды
  emit operationFinished(ReturnStatus);
}

void PersoClient::requestAuthorize(
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
  CurrentResponseDocument = QJsonDocument::fromJson(ReceivedDataBlock, &status);

  // Если пришел некорректный JSON
  if (status.error != QJsonParseError::NoError) {
    emit logging("Ошибка парсинга JSON команды. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  emit logging(
      QString("Размер ответа на команду: %1. Содержание ответа: %2. ")
          .arg(QString::number(CurrentResponseDocument.toJson().size()),
               QString(CurrentResponseDocument.toJson())));

  // Выделяем список пар ключ-значение из JSON-файла
  CurrentResponse = CurrentResponseDocument.object();
}

void PersoClient::createDataBlock(void) {
  emit logging("Формирование блока данных для команды. ");
  emit logging(QString("Размер команды: %1. Содержание команды: %2. ")
                   .arg(QString::number(CurrentCommandDocument.toJson().size()),
                        QString(CurrentCommandDocument.toJson())));

  // Инициализируем блок данных и сериализатор
  TransmittedDataBlock.clear();
  QDataStream serializator(&TransmittedDataBlock, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_5_12);

  // Формируем единый блок данных для отправки
  serializator << uint32_t(0) << CurrentCommandDocument.toJson();
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
  CurrentCommand["CommandName"] = "Echo";

  // Тело команды
  CurrentCommand["EchoData"] = "TestData";
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::createAuthorizationRequest(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды Authorization. ");

  // Заголовок команды
  CurrentCommand["CommandName"] = "Authorization";

  // Тело команды
  CurrentCommand["Login"] = requestAttributes->value("Login");
  CurrentCommand["Password"] = requestAttributes->value("Password");
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::createTransponderRelease(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRelease. ");

  // Заголовок команды
  CurrentCommand["CommandName"] = "TransponderRelease";

  // Тело команды
  CurrentCommand["Login"] = requestAttributes->value("Login");
  CurrentCommand["Password"] = requestAttributes->value("Password");
  CurrentCommand["UCID"] = requestAttributes->value("UCID");
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::createTransponderReleaseConfirm(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderReleaseConfirm. ");

  // Заголовок команды
  CurrentCommand["CommandName"] = "TransponderReleaseConfirm";

  // Тело команды
  CurrentCommand["Login"] = requestAttributes->value("Login");
  CurrentCommand["Password"] = requestAttributes->value("Password");
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::createTransponderRerelease(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRerelease. ");

  // Заголовок команды
  CurrentCommand["CommandName"] = "TransponderRerelease";

  // Тело команды
  CurrentCommand["Login"] = requestAttributes->value("Login");
  CurrentCommand["Password"] = requestAttributes->value("Password");
  CurrentCommand["UCID"] = requestAttributes->value("UCID");
  CurrentCommand["PAN"] = requestAttributes->value("PAN");
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::createTransponderRereleaseConfirm(
    const QMap<QString, QString>* requestAttributes) {
  emit logging("Формирование команды TransponderRereleaseConfirm. ");

  // Заголовок команды
  CurrentCommand["CommandName"] = "TransponderRereleaseConfirm";

  // Тело команды
  CurrentCommand["Login"] = requestAttributes->value("Login");
  CurrentCommand["Password"] = requestAttributes->value("Password");
  CurrentCommand["UCID"] = requestAttributes->value("UCID");
  CurrentCommand["PAN"] = requestAttributes->value("PAN");
  CurrentCommandDocument.setObject(CurrentCommand);

  // Создаем блок данных для команды
  createDataBlock();
}

void PersoClient::processEchoResponse(void) {
  emit logging("Обработка ответа на команду Echo. ");

  if (CurrentResponse.value("EchoData").isUndefined() ||
      CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() != "Echo")) {
    emit logging(
        "Обнаружена синтаксическая ошибка в команде EchoRequest: отсутствуют "
        "эхо-данные. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  emit logging("Команда EchoRequest успешно выполнена. ");
  ReturnStatus = CompletedSuccessfully;
}

void PersoClient::processAuthorizationResponse(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Синтаксическая проверка
  if (CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() != "Authorization") ||
      CurrentResponse.value("Access").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе Authorization. Сброс. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  responseAttributes->insert("Access",
                             CurrentResponse.value("Access").toString());

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда Authorization успешно выполнена. ");
}

void PersoClient::processTransponderReleaseResponse(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRelease. ");

  // Синтаксическая проверка
  if (CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() !=
       "TransponderRelease") ||
      CurrentResponse.value("Firmware").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderRelease: "
        "отсутствует файл прошивки. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(CurrentResponse.value("Firmware").toString().toUtf8());
  firmware->close();

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderRelease успешно выполнена. ");
}

void PersoClient::processTransponderReleaseConfirmResponse(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderReleaseConfirm. ");

  // Синтаксическая проверка
  if (CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() ==
       "TransponderReleaseConfirm") ||
      CurrentResponse.value("SN").isUndefined() ||
      CurrentResponse.value("PAN").isUndefined() ||
      CurrentResponse.value("BoxId").isUndefined() ||
      CurrentResponse.value("PalletId").isUndefined() ||
      CurrentResponse.value("OrderId").isUndefined() ||
      CurrentResponse.value("IssuerName").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderReleaseConfirm. "
        "Сброс. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  responseAttributes->insert("SN", CurrentResponse.value("SN").toString());
  responseAttributes->insert("PAN", CurrentResponse.value("PAN").toString());
  responseAttributes->insert("BoxId",
                             CurrentResponse.value("BoxId").toString());
  responseAttributes->insert("PalletId",
                             CurrentResponse.value("PalletId").toString());
  responseAttributes->insert("OrderId",
                             CurrentResponse.value("OrderId").toString());
  responseAttributes->insert("IssuerName",
                             CurrentResponse.value("IssuerName").toString());

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderReleaseConfirm успешно выполнена. ");
}

void PersoClient::processTransponderRereleaseResponse(QFile* firmware) {
  emit logging("Обработка ответа на команду TransponderRerelease. ");

  // Синтаксическая проверка
  if (CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() !=
       "TransponderRerelease") ||
      CurrentResponse.value("Firmware").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе TransponderRerelease: "
        "отсутствует файл прошивки. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware->open(QIODevice::WriteOnly)) {
    emit logging("Не удалось сохранить файл прошивки. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  // Сохраняем прошивку в файл
  firmware->write(CurrentResponse.value("Firmware").toString().toUtf8());
  firmware->close();

  ReturnStatus = CompletedSuccessfully;
  emit logging("Команда TransponderRerelease успешно выполнена. ");
}

void PersoClient::processTransponderRereleaseConfirmResponse(
    QMap<QString, QString>* responseAttributes) {
  emit logging("Обработка ответа на команду TransponderRereleaseConfirm. ");

  // Синтаксическая проверка
  if (CurrentResponse.value("ResponseName").isUndefined() ||
      (CurrentResponse.value("ResponseName").toString() ==
       "TransponderRereleaseConfirm") ||
      CurrentResponse.value("SN").isUndefined() ||
      CurrentResponse.value("PAN").isUndefined() ||
      CurrentResponse.value("BoxId").isUndefined() ||
      CurrentResponse.value("PalletId").isUndefined() ||
      CurrentResponse.value("OrderId").isUndefined() ||
      CurrentResponse.value("IssuerName").isUndefined()) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе "
        "TransponderRereleaseConfirm. "
        "Сброс. ");
    ReturnStatus = ResponseProcessingError;
    return;
  }

  responseAttributes->insert("SN", CurrentResponse.value("SN").toString());
  responseAttributes->insert("PAN", CurrentResponse.value("PAN").toString());
  responseAttributes->insert("BoxId",
                             CurrentResponse.value("BoxId").toString());
  responseAttributes->insert("PalletId",
                             CurrentResponse.value("PalletId").toString());
  responseAttributes->insert("OrderId",
                             CurrentResponse.value("OrderId").toString());
  responseAttributes->insert("IssuerName",
                             CurrentResponse.value("IssuerName").toString());

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
