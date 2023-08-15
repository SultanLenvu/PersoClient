#include "perso_client.h"

PersoClient::PersoClient(QObject *parent) : QObject(parent)
{
  setObjectName("PersoClient");
  applySettings();

  Firmware = nullptr;

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
  connect(Socket, &QTcpSocket::readyRead, WaitingLoop, &QEventLoop::quit);
  connect(Socket, &QTcpSocket::disconnected, WaitingLoop, &QEventLoop::quit);
  connect(Socket,
          QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
          WaitingLoop, &QEventLoop::quit);
  connect(WaitTimer, &QTimer::timeout, WaitingLoop, &QEventLoop::quit);
}

PersoClient::~PersoClient() {
  if (Socket->isOpen())
    Socket->disconnectFromHost();
}

QFile* PersoClient::getFirmware() {
  return Firmware;
}

void PersoClient::applySettings() {
  QSettings settings;

  PersoServerAddress =
      settings.value("Personalization/ServerIpAddress").toString();
  PersoServerPort = settings.value("Personalization/ServerPort").toInt();
}

void PersoClient::connectToPersoServer() {
  processingServerConnection();
}

void PersoClient::disconnectFromPersoServer() {
  emit logging("Отключение от сервера персонализации. ");

  Socket->disconnectFromHost();
}

void PersoClient::requestEcho() {
  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    return;
  }

  QJsonObject json;
  emit logging("Формирование команды EchoRequest. ");

  // Заголовок команды
  json["CommandName"] = "EchoRequest";

  // Тело команды
  json["EchoData"] = "TestData";

  CurrentCommand.setObject(json);

  // Сериализуем и отправляем команду
  transmitSerializedData();

  // Отключаемся от сервера
  Socket->close();
}

void PersoClient::requestFirmware() {
  // Подключаемся к серверу персонализации
  if (!processingServerConnection()) {
    return;
  }

  QJsonObject json;
  emit logging("Формирование команды FirmwareRequest. ");

  // Заголовок команды
  json["CommandName"] = "FirmwareRequest";

  // Тело команды
  json["UCID"] = "123456789";

  CurrentCommand.setObject(json);

  // Сериализуем и отправляем команду
  transmitSerializedData();

  // Отключаемся от сервера
  Socket->close();
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
        "Не удалось установить соединение с сервером персонализации. ");
    return false;
  }

  return true;
}

void PersoClient::processingReceivedDataBlock(QByteArray* dataBlock) {
  QJsonParseError status;
  CurrentResponse = QJsonDocument::fromJson(*dataBlock, &status);

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
  } else if (CurrentCommand.object().value("CommandName").toString() ==
             "FirmwareRequest") {
    processingFirmwareResponse(dataBlock);
  } else {
    emit logging(
        "Обнаружена синтаксическая ошибка: получена неизвестная команда. ");
    return;
  }
}

void PersoClient::transmitSerializedData() {
  QByteArray serializedData;
  QDataStream serializator(&serializedData, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_5_12);

  emit logging("Отправляемый блок данных: " +
               CurrentCommand.toJson(QJsonDocument::Compact));

  emit logging(
      QString::number(CurrentCommand.toJson(QJsonDocument::Compact).size()));

  // Формируем блок данных для отправки
  serializator << uint32_t(0);
  serializator << CurrentCommand.toJson(QJsonDocument::Compact);
  serializator.device()->seek(0);
  serializator << uint32_t(serializedData.size() - sizeof(uint32_t));

  emit logging(
      QString::number(uint32_t(serializedData.size() - sizeof(uint32_t))));

  // Отправляем сформируем блок данных
  Socket->write(serializedData);

  // Ожидаем ответа или отказа
  emit logging("Ожидание ответа от сервера. ");
  WaitTimer->start();
  WaitingLoop->exec();
}

void PersoClient::processingEchoResponse(QJsonObject* responseJson) {
  emit logging("Обработка ответа на команду EchoRequest. ");

  if (responseJson->value("EchoData") != QJsonValue::Undefined) {
    emit logging(QString("Полученные эхо-данные: %1.")
                     .arg(responseJson->value("EchoData").toString()));
    emit logging("Команда EchoRequest успешно выполнена. ");
  } else {
    emit logging(
        "Обнаружена синтаксическая ошибка в команде EchoRequest: отсутствуют "
        "эхо-данные. ");
  }
}

void PersoClient::processingFirmwareResponse(QByteArray* firmware) {
  emit logging("Обработка ответа на команду FirmwareRequest. ");

  Firmware = new QFile("firmware.hex", this);
  if (Firmware->open(QIODevice::WriteOnly)) {
    Firmware->write(*firmware);
    Firmware->close();
  } else {
    emit logging("Не удалось сохранить файл прошивки. ");
  }

  delete Firmware;
  Firmware = nullptr;
}

void PersoClient::deleteFirmware() {
  if (!Firmware)
    return;

  if (!Firmware->exists())
    return;

  emit logging("Файл прошивки удален. ");
  Firmware->remove();
  delete Firmware;
  Firmware = nullptr;
}

void PersoClient::on_SocketDisconnected_slot() {
  emit logging("Соединение с сервером персонализации отключено. ");
}

void PersoClient::on_SocketConnected_slot() {
  emit logging("Соединение с сервером персонализации установлено. ");
}

void PersoClient::on_SocketReadyRead_slot() {
  QByteArray ReceivedDataBlock;
  uint32_t blockSize = 0;              // Размер блока
  QDataStream deserializator(Socket);  // Дессериализатор
  deserializator.setVersion(
      QDataStream::Qt_5_12);  // Настраиваем версию десериализатора

  // Цикл формирования блока данных
  while (true) {
    // Если блок данных еще не начал формироваться
    if (!blockSize) {
      // Если размер поступивших байт меньше размера поля с размером байт, то
      // блок поступившие данные отбрасываются
      if (Socket->bytesAvailable() < sizeof(uint32_t)) {
        break;
      }
      deserializator >> blockSize;

      emit logging(QString("Размер полученного блока данных: %1.")
                       .arg(QString::number(blockSize)));
    }

    emit logging(QString("Размер принятых данных: %1. ")
                     .arg(QString::number(Socket->bytesAvailable())));

    // Дожидаемся пока весь блок данных придет целиком
    if (Socket->bytesAvailable() < blockSize) {
      emit logging(
          "Блок получен не целиком. Ожидается прием следующих частей. ");
      break;
    }

    // Если блок был получен целиком, то осуществляем его дессериализацию
    deserializator >> ReceivedDataBlock;
    emit logging("Блок полученных данных: " + ReceivedDataBlock);

    // Выходим
    break;
  }

  // Осуществляем обработку полученных данных
  processingReceivedDataBlock(&ReceivedDataBlock);
}

void PersoClient::on_SocketError_slot(
    QAbstractSocket::SocketError socketError) {
  switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
      emit logging("Ошибка сети: запрос на подключение отклонен сервером. ");
      break;

    case QAbstractSocket::RemoteHostClosedError:
      emit logging("Ошибка сети: сервер закрыл соединение. ");
      break;

    case QAbstractSocket::HostNotFoundError:
      emit logging("Ошибка сети: сервер не найден. ");
      break;

    case QAbstractSocket::SocketAccessError:
      emit logging(
          "Ошибка сети: подключение к серверу не выполнено из-за нехватки прав "
          "доступа. ");
      break;

    case QAbstractSocket::SocketResourceError:
      emit logging(
          "Ошибка сети: подключение к серверу не выполнено из-за перезгрузки "
          "операционной "
          "системы. ");
      break;

    case QAbstractSocket::QAbstractSocket::SocketTimeoutError:
      emit logging(
          "Ошибка сети: исстекло время на выполнение сетевой операции. ");
      break;

    default:
      emit logging("Ошибка сети: неизвестная ошибка. ");
      break;
  }
}

void PersoClient::on_WaitTimerTimeout_slot() {
  emit logging("Время ожидания вышло. ");
  Socket->close();
}
