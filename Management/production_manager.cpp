#include "production_manager.h"
#include "definitions.h"
#include "global_environment.h"
#include "jlink_exe_programmer.h"
#include "log_system.h"
#include "perso_server_connection.h"
#include "te310_printer.h"

ProductionManager::ProductionManager(const QString& name)
    : AbstractManager{name} {
  loadSettings();
}

ProductionManager::~ProductionManager() {}

void ProductionManager::onInstanceThreadStarted() {
  createProgrammer();
  createServer();
  createStickerPrinter();
}

AbstractManager::Type ProductionManager::type() const {
  return Type::Production;
}

void ProductionManager::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();

  Server->applySettings();
  Programmer->applySettings();
  StickerPrinter->applySetting();
}

void ProductionManager::connectToServer() {
  emit executionStarted("connectToServer");
  sendLog("Подключение серверу сервером. ");

  ReturnStatus ret;
  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("connectToServer", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Соединение с сервером успешно инициализировано. ");
  emit executionFinished("connectToServer", ret);
}

void ProductionManager::disconnectFromServer() {
  emit executionStarted("connectToServer");
  sendLog("Отключение от сервера. ");

  Server->disconnect();

  // Завершаем операцию
  sendLog("Соединение с сервером отключено. ");
  emit executionFinished("connectToServer", ReturnStatus::NoError);
}

void ProductionManager::launchProductionLine(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("launchProductionLine");
  sendLog("Инициализация соединения с сервером. ");

  ReturnStatus ret;
  ret = Server->logIn(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("launchProductionLine", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Соединение с сервером успешно инициализировано. ");
  emit executionFinished("launchProductionLine", ret);
}

void ProductionManager::resetServerConnection() {
  emit executionStarted("resetServerConnection");
  sendLog("Сброс соединения с сервером. ");

  Server->disconnect();
  Server->logOut();

  sendLog("Соединение с сервером успешно сброшено. ");
  emit executionFinished("resetServerConnection", ReturnStatus::NoError);
}

void ProductionManager::echoServer() {
  emit executionStarted("echoServer");
  sendLog("Выполнение эхо-запроса. ");

  ReturnStatus ret;
  ret = Server->echo();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("echoServer", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Эхо-запрос успешно выполнен. ");
  emit executionFinished("echoServer", ret);
}

void ProductionManager::requestBox() {
  emit executionStarted("requestBox");
  sendLog("Выполнение запроса бокса. ");

  ReturnStatus ret;

  ret = Server->requestBox();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("requestBox", ret);
    sendLog("Не удалось получить бокс для сборки. ");
    return;
  }

  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("requestBox", ret);
    sendLog("Не удалось получить данные текущего бокса. ");
    return;
  }

  emit displayBoxData_signal(BoxData);

  // Запрашиваем данные текущего транспондера
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("launchProductionLine", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Бокс для сборки получен. ");
  emit executionFinished("requestBox", ret);
}

void ProductionManager::getCurrentBoxData() {
  emit executionStarted("getCurrentBoxData");
  sendLog("Выполнение запроса бокса. ");

  ReturnStatus ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("getCurrentBoxData", ret);
    sendLog("Не удалось получить данные текущего бокса. ");
    return;
  }

  emit displayBoxData_signal(BoxData);

  // Завершаем операцию
  sendLog("Данные текущего бокса получены. ");
  emit executionFinished("getCurrentBoxData", ret);
}

void ProductionManager::refundCurrentBox() {
  emit executionStarted("refundCurrentBox");
  sendLog("Возврат текущего бокса. ");

  ReturnStatus ret;
  ret = Server->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("refundCurrentBox", ret);
    sendLog("Не удалось вернуть текущий бокс для сборки. ");
    return;
  }

  // Проверить, что будет если запросить данные бокса после возврата
  //  ret = Server->getCurrentBoxData(BoxData);
  //  if (ret != ReturnStatus::NoError) {
  //    emit executionFinished("refundCurrentBox", ret);
  //    sendLog("Не удалось получить данные текущего бокса. ");
  //    return;
  //  }

  BoxData.clear();
  emit displayBoxData_signal(BoxData);

  // Завершаем операцию
  sendLog("Текущий бокс успешно возвращен. ");
  emit executionFinished("refundCurrentBox", ret);
}

void ProductionManager::completeCurrentBox() {
  emit executionStarted("completeCurrentBox");
  sendLog("Завершение сборки текущего бокса. ");

  ReturnStatus ret;
  ret = Server->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("completeCurrentBox", ret);
    sendLog("Не удалось завершить сборку текущего бокса. ");
    return;
  }

  BoxData.clear();
  emit displayBoxData_signal(BoxData);

  // Завершаем операцию
  sendLog("Сборка текуего бокса успешно завершена. ");
  emit executionFinished("completeCurrentBox", ret);
}

void ProductionManager::releaseTransponder() {
  emit executionStarted("releaseTransponder");
  sendLog("Выпуск транспондера. ");

  ReturnStatus ret;
  QString ucid;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);
  StringDictionary result;
  StringDictionary param;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Считываем UCID
  ret = Programmer->readUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Выпускаем транспондер
  ret = Server->releaseTransponder(result);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    sendLog("Не удалось сохранить файл прошивки. ");
    emit executionFinished("releaseTransponder", ReturnStatus::FileOpenError);
  }

  // Сохраняем прошивку в файл
  firmware.write(QByteArray::fromBase64(result.value("firmware").toUtf8()));
  firmware.close();

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Удаляем файл прошивки
  firmware.remove();

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Подтверждаем выпуск транспондера
  param.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRelease(param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Запрашиваем данные очередного транспондера
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Транспондер успешно выпущен. ");
  emit executionFinished("releaseTransponder", ret);
}

void ProductionManager::rereleaseTransponder(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("rereleaseTransponder");
  sendLog("Выпуск транспондера. ");

  ReturnStatus ret;
  QString ucid;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);
  StringDictionary result;
  StringDictionary requestParam;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Считываем UCID
  ret = Programmer->readUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Перевыпускаем транспондер
  requestParam.insert("transponder_pan", param->value("transponder_pan"));
  ret = Server->rereleaseTransponder(requestParam, result);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    sendLog("Не удалось сохранить файл прошивки. ");
    emit executionFinished("rereleaseTransponder", ReturnStatus::FileOpenError);
  }

  // Сохраняем прошивку в файл
  firmware.write(QByteArray::fromBase64(result.value("firmware").toUtf8()));
  firmware.close();

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Удаляем файл прошивки
  firmware.remove();

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Подтверждаем перевыпуск транспондера
  requestParam.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRerelease(requestParam);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rereleaseTransponder", ret);
    return;
  }

  // Запрашиваем данные перевыпущенного транспондера
  requestParam.remove("transponder_ucid");
  ret = Server->getTransponderData(requestParam, TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Транспондер успешно выпущен. ");
  emit executionFinished("rereleaseTransponder", ret);
}

void ProductionManager::rollbackTransponder() {
  emit executionStarted("rollbackTransponder");
  sendLog("Откат производственной линии. ");

  ReturnStatus ret;
  ret = Server->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("rollbackTransponder", ret);
    return;
  }

  // Запрашиваем данные очередного транспондера
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("releaseTransponder", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Откат производственной линии успешно завершен. ");
  emit executionFinished("rollbackTransponder", ret);
}

void ProductionManager::getCurrentTransponderData() {
  emit executionStarted("getCurrentTransponderData");
  sendLog("Запрос данных текущего транспондера. ");

  ReturnStatus ret;
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("getCurrentTransponderData", ret);
    sendLog("Не удалось получить данные текущего транспондера. ");
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Данные текущего транспондера успешно получены. ");
  emit executionFinished("getCurrentTransponderData", ret);
}

void ProductionManager::getTransponderData(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("getTransponderData");
  sendLog("Запрос данных транспондера. ");

  ReturnStatus ret;
  ret = Server->getTransponderData(*param.get(), TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("getTransponderData", ret);
    sendLog("Не удалось получить данные транспондера. ");
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Данные транспондера успешно получены. ");
  emit executionFinished("getTransponderData", ret);
}

void ProductionManager::printBoxSticker(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("printBoxSticker");
  sendLog("Печать стикера для бокса. ");

  ReturnStatus ret;
  ret = Server->printBoxSticker(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printBoxSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Стикер для бокса успешно распечатан. ");
  emit executionFinished("printBoxSticker", ret);
}

void ProductionManager::printLastBoxSticker() {
  emit executionStarted("printLastBoxSticker");
  sendLog("Печать последнего стикера для бокса. ");

  ReturnStatus ret;
  ret = Server->printLastBoxSticker();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printLastBoxSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Последний стикер для бокса успешно распечатан. ");
  emit executionFinished("printLastBoxSticker", ret);
}

void ProductionManager::printPalletSticker(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("printPalletSticker");
  sendLog("Печать стикера для паллеты. ");

  ReturnStatus ret;
  ret = Server->printPalletSticker(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printPalletSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Стикер для паллеты успешно распечатан. ");
  emit executionFinished("printPalletSticker", ret);
}

void ProductionManager::printLastPalletSticker() {
  emit executionStarted("printLastPalletSticker");
  sendLog("Печать последнего стикера для паллеты. ");

  ReturnStatus ret;
  ret = Server->printLastPalletSticker();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printLastPalletSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Последний стикер для паллеты успешно распечатан. ");
  emit executionFinished("printLastPalletSticker", ret);
}

void ProductionManager::loadSettings() {}

void ProductionManager::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

void ProductionManager::createProgrammer() {
  Programmer = std::unique_ptr<AbstractProgrammer>(
      new JLinkExeProgrammer("JLinkExeProgrammer1"));
  connect(Programmer.get(), &AbstractProgrammer::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}

void ProductionManager::createServer() {
  Server = std::unique_ptr<AbstractServerConnection>(
      new PersoServerConnection("PersoServerConnection"));
  connect(Server.get(), &AbstractServerConnection::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}

void ProductionManager::createStickerPrinter() {
  StickerPrinter =
      std::unique_ptr<AbstractStickerPrinter>(new TE310Printer("TSC TE310"));
  connect(StickerPrinter.get(), &AbstractStickerPrinter::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}
