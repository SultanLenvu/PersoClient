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

  createProgrammer();
  createServer();
  createStickerPrinter();

  TransponderData = std::shared_ptr<StringDictionary>(new StringDictionary());
}

ProductionManager::~ProductionManager() {}

void ProductionManager::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();

  Server->applySettings();
  Programmer->applySettings();
  StickerPrinter->applySetting();
}

void ProductionManager::initServerConnection(
    const std::shared_ptr<StringDictionary> param) {
  emit executionStarted("initServerConnection");
  sendLog("Инициализация соединения с сервером. ");

  ReturnStatus ret;
  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("initServerConnection", ret);
    return;
  }

  ret = Server->logIn(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("initServerConnection", ret);
    return;
  }

  // Запрашиваем данные текущего транспондера
  ret = Server->getCurrentTransponderData(*TransponderData);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("initServerConnection", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Завершаем операцию
  sendLog("Соединение с сервером успешно инициализировано. ");
  emit executionFinished("initServerConnection", ret);
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
  ret = StickerPrinter->printTransponderSticker(*TransponderData);
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
  ret = Server->getCurrentTransponderData(*TransponderData);
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
  ret = StickerPrinter->printTransponderSticker(*TransponderData);
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
  ret = Server->getTransponderData(requestParam, *TransponderData);
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
  ret = Server->getCurrentTransponderData(*TransponderData);
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
