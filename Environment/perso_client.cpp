#include "perso_client.h"

PersoClient::PersoClient(QObject *parent) : QObject(parent)
{
  setObjectName("PersoClient");
  applySettings();

  FirmwareFile = nullptr;

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
  connect(this, &PersoClient::stopWaiting, WaitingLoop, &QEventLoop::quit);

  // Команды еще не выполнялись
  CurrentCommandStatus = NotExecuted;
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
  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    emit operationFinished(OperationStatus::Failed);
  } else {
    emit operationFinished(OperationStatus::Success);
  }
}

void PersoClient::disconnectFromPersoServer() {
  if (Socket->isOpen()) {
    Socket->disconnectFromHost();
    emit logging("Отключение от сервера персонализации. ");
    emit operationFinished(OperationStatus::Success);
  } else {
    emit logging("Подключение не было установлено. ");
    emit operationFinished(OperationStatus::Failed);
  }
}

void PersoClient::requestEcho() {
  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    emit operationFinished(OperationStatus::Failed);
    return;
  }

  QJsonObject json;
  emit logging("Формирование команды EchoRequest. ");

  // Заголовок команды
  json["CommandName"] = "EchoRequest";

  // Тело команды
  json["EchoData"] = "TestData";

  CurrentCommand.setObject(json);

  // Ответный блок данных еще не получен
  ReceivedDataBlockSize = 0;

  // Создаем блок данных для команды
  createDataBlock();

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();

  // Возвращаем статус выполнения команды
  if (CurrentCommandStatus == Completed) {
    emit operationFinished(OperationStatus::Success);
  } else {
    emit operationFinished(OperationStatus::Failed);
  }
}

void PersoClient::requestFirmware(QFile* firmware) {
  // Проверка файла
  if (!checkFirmwareFile(firmware)) {
    emit operationFinished(OperationStatus::Failed);
    return;
  }

  // Запоминаем файл
  FirmwareFile = firmware;

  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    emit operationFinished(OperationStatus::Failed);
    return;
  }

  QJsonObject json;
  emit logging("Формирование команды FirmwareRequest. ");

  // Заголовок команды
  json["CommandName"] = "FirmwareRequest";

  // Тело команды
  json["UCID"] = "123456789";

  CurrentCommand.setObject(json);

  // Ответный блок данных еще не получен
  ReceivedDataBlockSize = 0;

  // Создаем блок данных для команды
  createDataBlock();

  // Отправляем сформированный блок данных
  transmitDataBlock();

  // Отключаемся от сервера
  Socket->close();

  // Возвращаем статус выполнения команды
  if (CurrentCommandStatus == Completed) {
    emit operationFinished(OperationStatus::Success);
  } else {
    emit operationFinished(OperationStatus::Failed);
  }
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
  CurrentResponse = QJsonDocument::fromJson(ReceivedDataBlock, &status);

  // Если пришел некорректный JSON
  if (status.error != QJsonParseError::NoError) {
    emit logging("Ошибка парсинга JSON команды. ");
    return;
  }

  // Выделяем список пар ключ-значение из JSON-файла
  QJsonObject json = CurrentResponse.object();

  // Синтаксическая проверка
  if (json.value("CommandName") == QJsonValue::Undefined) {
    emit logging(
        "Обнаружена синтаксическая ошибка: в запросе отсутствует название "
        "команды. ");
    return;
  }

  // Вызываем соответствующий обработчик команды
  if (json.value("CommandName").toString() == "EchoResponse") {
    processingEchoResponse(&json);
  } else if (json.value("CommandName").toString() == "FirmwareResponse") {
    processingFirmwareResponse(&json);
  } else {
    emit logging(
        "Обнаружена синтаксическая ошибка: получена неизвестная команда. ");
    return;
  }
}

void PersoClient::createDataBlock() {
  emit logging("Формирование блока данных для команды. ");
  emit logging(
      QString("Размер команды: %1. Содержание команды: %2. ")
          .arg(QString::number(
              CurrentCommand.toJson(QJsonDocument::Compact).size()))
          .arg(QString(CurrentCommand.toJson(QJsonDocument::Compact))));

  // Инициализируем блок данных и сериализатор
  TransmittedDataBlock.clear();
  QDataStream serializator(&TransmittedDataBlock, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_5_12);

  // Формируем единый блок данных для отправки
  serializator << uint32_t(0) << CurrentCommand.toJson(QJsonDocument::Compact);
  serializator.device()->seek(0);
  serializator << uint32_t(TransmittedDataBlock.size() - sizeof(uint32_t));
}

void PersoClient::transmitDataBlock() {
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

void PersoClient::processingEchoResponse(QJsonObject* responseJson) {
  emit logging("Обработка ответа на команду EchoRequest. ");

  if (responseJson->value("EchoData") != QJsonValue::Undefined) {
    emit logging(QString("Полученные эхо-данные: %1.")
                     .arg(responseJson->value("EchoData").toString()));
    emit logging("Команда EchoRequest успешно выполнена. ");
    CurrentCommandStatus = Completed;
  } else {
    emit logging(
        "Обнаружена синтаксическая ошибка в команде EchoRequest: отсутствуют "
        "эхо-данные. ");
  }
}

void PersoClient::processingFirmwareResponse(QJsonObject* responseJson) {
  // Блокируем доступ к файлу
  Mutex.lock();

  emit logging("Обработка ответа на команду FirmwareRequest. ");

  // Синтаксическая проверка
  if (responseJson->value("FirmwareFile") == QJsonValue::Undefined) {
    emit logging(
        "Обнаружена синтаксическая ошибка в ответе FirmwareResponse: "
        "отсутствует файл прошивки. ");
    return;
  }

  // Сохраняем присланный файл прошивки
  if (FirmwareFile->open(QIODevice::WriteOnly)) {
    FirmwareFile->write(QByteArray::fromBase64(
        responseJson->value("FirmwareFile").toString().toUtf8()));
    FirmwareFile->close();
  } else {
    emit logging("Не удалось сохранить файл прошивки. ");
  }

  CurrentCommandStatus = Completed;
  emit logging("Команда FirmwareRequest успешно выполнена. ");

  // Разблокирует доступ к файлу
  Mutex.unlock();
}

bool PersoClient::checkFirmwareFile(QFile* firmware) {
  if (firmware == nullptr) {
    emit logging("Файл прошивки не был создан. ");
    return false;
  }

  QFileInfo info(*firmware);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile())) {
    emit logging("Файл прошивки не корректен. ");
    return false;
  }

  // Проврека на размер
  if ((info.size() > FIRMWARE_FILE_MAX_SIZE) || (info.size() == 0)) {
    emit logging("Файл прошивки имеет слишком большой размер. ");
    return false;
  }

  emit logging("Файл прошивки корректен. ");
  return true;
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
    if (Socket->bytesAvailable() < sizeof(uint32_t)) {
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
      emit stopWaiting();
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
  emit logging("Блок полученных данных: " + ReceivedDataBlock);

  // Останавливаем цикл ожидания
  emit stopWaiting();

  // Осуществляем обработку полученных данных
  processingDataBlock();
}

void PersoClient::on_SocketError_slot(
    QAbstractSocket::SocketError socketError) {
  emit logging(
      QString("Ошибка сети: Код: %1. Описание: %2.")
          .arg(QString::number(socketError).arg(Socket->errorString())));
  Socket->close();
  CurrentCommandStatus = Failed;
}

void PersoClient::on_WaitTimerTimeout_slot() {
  emit logging("Время ожидания вышло. ");
  CurrentCommandStatus = Failed;
  Socket->close();
}
