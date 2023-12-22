#include <QDataStream>
#include <QSettings>

#include "print_box_sticker.h"
#include "definitions.h"
#include "echo.h"
#include "print_last_box_sticker.h"
#include "print_last_pallet_sticker.h"
#include "log_in.h"
#include "log_out.h"
#include "print_pallet_sticker.h"
#include "perso_server_connection.h"
#include "release_transponder.h"
#include "confirm_transponder_release.h"
#include "rerelease_transponder.h"
#include "confirm_transponder_rerelease.h"
#include "rollback_transponder.h"
#include "get_current_transponder_data.h"

PersoServerConnection::PersoServerConnection(const QString& name)
    : AbstractServerConnection(name) {
  loadSettings();

  // Создаем сокет
  createSocket();

  // Создаем таймеры
  createTimers();

  // Создаем команды
  createCommands();
}

PersoServerConnection::~PersoServerConnection() {
  if (Socket->isOpen()) {
    Socket->disconnectFromHost();
  }
}

ReturnStatus PersoServerConnection::connect() {
  sendLog("Подключение к серверу персонализации. ");
  // Подключаемся к серверу персонализации
  Socket->connectToHost(PersoServerAddress, PersoServerPort);

  // Ожидаем подключения или отказа
  sendLog("Ожидание ответа от сервера. ");
  WaitTimer->start();
  WaitingLoop->exec();

  // Если соединение не удалось установить
  if (!Socket->isOpen()) {
    sendLog(
        "Не удалось установить соединение с сервером персонализации. Сброс. ");
    return ReturnStatus::ServerConnectionError;
  }

  return ReturnStatus::NoError;
}

void PersoServerConnection::disconnect() {
  if (!Socket->isOpen()) {
    sendLog("Подключение не было установлено. ");
    return;
  }

  sendLog("Отключение от сервера персонализации. ");
  Socket->disconnectFromHost();
}

ReturnStatus PersoServerConnection::echo() {
  CurrentCommand = Commands.at(Echo);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::logIn(const StringDictionary& param) {
  CurrentCommand = Commands.at(LogIn);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(param, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::logOut() {
  CurrentCommand = Commands.at(LogOut);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::update(StringDictionary& context) {
  CurrentCommand = Commands.at(Update);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock, context);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
              QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::getTransponderData(const StringDictionary &param, StringDictionary &result)
{

}

ReturnStatus PersoServerConnection::release(StringDictionary& result) {
  CurrentCommand = Commands.at(Release);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock, result);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::confirmRelease(
    const StringDictionary& param) {
  CurrentCommand = Commands.at(ConfirmRelease);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(param, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::rerelease(const StringDictionary& param,
                                              StringDictionary& result) {
  CurrentCommand = Commands.at(Rerelease);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(param, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock, result);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::confirmRerelease(
    const StringDictionary& param) {
  CurrentCommand = Commands.at(ConfirmRerelease);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(param, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::rollback() {
  CurrentCommand = Commands.at(Rollback);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::printBoxSticker(
    const StringDictionary& param) {
  CurrentCommand = Commands.at(PrintBoxSticker);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(param, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::printLastBoxSticker() {
  CurrentCommand = Commands.at(PrintLastBoxSticker);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::printPalletSticker(
    const StringDictionary& data) {
  CurrentCommand = Commands.at(PrintPalletSticker);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(data, dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

ReturnStatus PersoServerConnection::printLastPalletSticker() {
  CurrentCommand = Commands.at(PrintLastPalletSticker);
  sendLog(
      QString("Начало выполнения команды '%1'.").arg(CurrentCommand->name()));

  QByteArray dataBlock;
  ReturnStatus ret = CurrentCommand->generate(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = transmitDataBlock(dataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  if (!waitResponse()) {
    return ReturnStatus::ServerNotResponding;
  }

  ret = CurrentCommand->processResponse(ReceivedDataBlock);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  sendLog(
      QString("Команда '%1' успешно выполнена.").arg(CurrentCommand->name()));
}

void PersoServerConnection::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();
}

void PersoServerConnection::loadSettings() {
  QSettings settings;

  PersoServerAddress =
      QHostAddress(settings.value("perso_server_connection/ip").toString());
  PersoServerPort = settings.value("perso_server_connection/port").toInt();
}

void PersoServerConnection::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

ReturnStatus PersoServerConnection::transmitDataBlock(
    const QByteArray& dataBlock) {
  ReturnStatus ret;
  // Ответный блок данных еще не получен
  ReceivedDataBlockSize = 0;

  // Проверяем подключение к серверу персонализации
  if (!Socket->isOpen()) {
    sendLog("Соединение с сервером персонализации не установлено.");
    return ReturnStatus::ServerConnectionError;
  }

  // Если размер блок не превышает максимального размера данных для единоразовой
  // передачи
  if (dataBlock.size() < ONETIME_TRANSMIT_DATA_SIZE) {
    // Отправляем блок данных
    if (Socket->write(dataBlock) == -1) {
      sendLog("Получена ошибка при отправке блока данных.");
      return ReturnStatus::ServerDataTransmittingError;
    }

  } else {
    // В противном случае дробим блок данных на части и последовательно
    // отправляем
    for (int32_t i = 0; i < dataBlock.size(); i += ONETIME_TRANSMIT_DATA_SIZE) {
      Socket->write(dataBlock.mid(i, ONETIME_TRANSMIT_DATA_SIZE));
    }
  }

  return ReturnStatus::NoError;
}

bool PersoServerConnection::waitResponse() {
  // Ожидаем ответ
  WaitTimer->start();
  WaitingLoop->exec();

  // Если сервер ничего не ответил
  if (ReceivedDataBlock.isEmpty()) {
    sendLog("Сервер не отвечает.");
    return false;
  }

  return true;
}

void PersoServerConnection::createTimers() {
  WaitTimer = std::unique_ptr<QTimer>(new QTimer());
  WaitTimer->setInterval(PERSO_SERVER_CONNECTION_WAITING_TIME);
  QObject::connect(WaitTimer.get(), &QTimer::timeout, this,
                   &PersoServerConnection::waitTimerTimeout_slot);
  QObject::connect(WaitTimer.get(), &QTimer::timeout, WaitTimer.get(),
                   &QTimer::stop);
  QObject::connect(Socket.get(), &QTcpSocket::readyRead, WaitTimer.get(),
                   &QTimer::stop);
  QObject::connect(Socket.get(), &QTcpSocket::connected, WaitTimer.get(),
                   &QTimer::stop);
  QObject::connect(Socket.get(), &QTcpSocket::disconnected, WaitTimer.get(),
                   &QTimer::stop);
  QObject::connect(Socket.get(), &QTcpSocket::errorOccurred, WaitTimer.get(),
                   &QTimer::stop);

  WaitingLoop = std::unique_ptr<QEventLoop>(new QEventLoop());
  QObject::connect(Socket.get(), &QTcpSocket::connected, WaitingLoop.get(),
                   &QEventLoop::quit);
  QObject::connect(Socket.get(), &QTcpSocket::disconnected, WaitingLoop.get(),
                   &QEventLoop::quit);
  QObject::connect(Socket.get(), &QTcpSocket::errorOccurred, WaitingLoop.get(),
                   &QEventLoop::quit);
  QObject::connect(WaitTimer.get(), &QTimer::timeout, WaitingLoop.get(),
                   &QEventLoop::quit);
  QObject::connect(this, &PersoServerConnection::stopResponseWaiting,
                   WaitingLoop.get(), &QEventLoop::quit);
}

void PersoServerConnection::createSocket(void) {
  Socket = std::unique_ptr<QTcpSocket>(new QTcpSocket());
  QObject::connect(Socket.get(), &QTcpSocket::connected, this,
                   &PersoServerConnection::socketConnected_slot);
  QObject::connect(Socket.get(), &QTcpSocket::disconnected, this,
                   &PersoServerConnection::socketDisconnected_slot);
  QObject::connect(Socket.get(), &QTcpSocket::readyRead, this,
                   &PersoServerConnection::socketReadyRead_slot);
  QObject::connect(Socket.get(), &QTcpSocket::errorOccurred, this,
                   &PersoServerConnection::socketError_slot);
}

void PersoServerConnection::createCommands() {}

void PersoServerConnection::socketConnected_slot() {
  sendLog("Соединение с сервером персонализации установлено. ");
}

void PersoServerConnection::socketDisconnected_slot() {
  sendLog("Соединение с сервером персонализации отключено. ");
}

void PersoServerConnection::socketReadyRead_slot() {
  QDataStream deserializator(Socket.get());  // Дессериализатор
  deserializator.setVersion(
      QDataStream::Qt_5_12);  // Настраиваем версию десериализатора

  // Если блок данных еще не начал формироваться
  if (ReceivedDataBlockSize == 0) {
    // Если размер поступивших байт меньше размера поля с размером байт, то
    // блок поступившие данные отбрасываются
    if (Socket->bytesAvailable() < static_cast<int64_t>(sizeof(uint32_t))) {
      sendLog(
          "Размер полученных данных слишком мал. Ожидается прием следующих "
          "частей. ");
      // Перезапускаем таймер ожидания для следующих частей
      WaitTimer->start();
      return;
    }
    // Сохраняем размер блока данных
    deserializator >> ReceivedDataBlockSize;

    sendLog(QString("Размер ожидаемого блока данных: %1.")
                .arg(QString::number(ReceivedDataBlockSize)));

    // Если размер блока данных слишком большой, то весь блок отбрасывается
    if (ReceivedDataBlockSize > DATA_BLOCK_MAX_SIZE) {
      sendLog("Размер блока данных слишком большой. Сброс. ");
      // Останавливаем цикл ожидания
      emit stopResponseWaiting();
      ReceivedDataBlockSize = 0;
    }
  }

  sendLog(QString("Размер принятых данных: %1. ")
              .arg(QString::number(Socket->bytesAvailable())));

  // Дожидаемся пока весь блок данных придет целиком
  if (Socket->bytesAvailable() < ReceivedDataBlockSize) {
    sendLog("Блок получен не целиком. Ожидается прием следующих частей. ");
    // Перезапускаем таймер ожидания для следующих частей
    WaitTimer->start();
    return;
  }

  // Если блок был получен целиком, то осуществляем его дессериализацию
  deserializator >> ReceivedDataBlock;

  // Останавливаем цикл ожидания
  emit stopResponseWaiting();
}

void PersoServerConnection::socketError_slot(
    QAbstractSocket::SocketError socketError) {
  Socket->abort();
  CurrentCommand->clear();
  sendLog(QString("Ошибка сети: %1. %2.")
              .arg(QString::number(socketError), Socket->errorString()));
}

void PersoServerConnection::waitTimerTimeout_slot() {
  Socket->abort();
  CurrentCommand->clear();
  sendLog("Время ожидания вышло. ");
}
