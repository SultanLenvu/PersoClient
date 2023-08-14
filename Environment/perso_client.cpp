#include "perso_client.h"

PersoClient::PersoClient(QObject *parent) : QObject(parent)
{
  setObjectName("PersoClient");
  Firmware = nullptr;

  PersoServerAddress = PERSO_SERVER_DEFAULT_IP;
  PersoServerPort = PERSO_SERVER_DEFAULT_PORT;

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
  emit logging("Подключение к серверу персонализации. ");

  Socket->connectToHost(PersoServerAddress, PersoServerPort);
}

void PersoClient::disconnectFromPersoServer() {
  emit logging("Отключение от сервера персонализации. ");

  Socket->disconnectFromHost();
}

void PersoClient::echoRequest() {
  if (!Socket->isOpen()) {
    emit logging("Соединение с сервером персонализации не установлено. ");
    return;
  }

  Socket->write("EchoTest");
}

void PersoClient::getFirmwareRequest() {}

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
  QByteArray ReceivedData = Socket->readAll();

  emit logging("Размер полученных данных: " +
               QString::number(ReceivedData.size()));

  deleteFirmware();
  Firmware = new QFile(FIRMWARE_TEMP_FILE_NAME);
  if (Firmware->open(QIODevice::WriteOnly)) {
    Firmware->write(ReceivedData);
    Firmware->close();
  } else {
    emit logging("Не удалось создать временный файл прошивки. ");
    deleteFirmware();
  }
}

void PersoClient::on_SocketError_slot(
    QAbstractSocket::SocketError socketError) {
  switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
      emit logging("Запрос на подключение отклонен сервером. ");
      break;

    case QAbstractSocket::RemoteHostClosedError:
      emit logging("Сервер закрыл соединение. ");
      break;

    case QAbstractSocket::HostNotFoundError:
      emit logging("Сервер не найден. ");
      break;

    case QAbstractSocket::SocketAccessError:
      emit logging(
          "Подключение к серверу не выполнено из-за нехватки прав доступа. ");
      break;

    case QAbstractSocket::SocketResourceError:
      emit logging(
          "Подключение к серверу не выполнено из-за перезгрузки операционной "
          "системы. ");
      break;

    case QAbstractSocket::QAbstractSocket::SocketTimeoutError:
      emit logging("Исстекло время на выполнение сетевой операции. ");
      break;

    default:
      emit logging("Неизвестная ошибка. ");
      break;
  }
}
