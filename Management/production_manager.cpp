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
  createServerConnection();
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
  initOperation("connectToServer");

  ReturnStatus ret;
  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    processOperationError("connectToServer", ret);
    return;
  }

  completeOperation("connectToServer");
}

void ProductionManager::disconnectFromServer() {
  initOperation("connectToServer");

  Server->disconnect();

  completeOperation("connectToServer");
}

void ProductionManager::launchProductionLine(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("launchProductionLine");

  ReturnStatus ret;
  ret = Server->launchProductionLine(*param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("launchProductionLine", ret);
    return;
  }

  completeOperation("launchProductionLine");
}

void ProductionManager::shutdownProductionLine() {
  initOperation("shutdownProductionLine");

  Server->shutdownProductionLine();

  completeOperation("shutdownProductionLine");
}

void ProductionManager::logOnServer(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("logOnServer");

  ReturnStatus ret;
  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOnServer", ret);
    return;
  }

  ret = Server->launchProductionLine(*param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("logOnServer", ret);
    return;
  }

  emit authorizationCompleted();
  completeOperation("logOnServer");
}

void ProductionManager::logOutServer() {
  initOperation("logOutServer");

  Server->shutdownProductionLine();
  Server->disconnect();

  completeOperation("logOutServer");
}

void ProductionManager::echoServer() {
  initOperation("echoServer");

  ReturnStatus ret;
  ret = Server->echo();
  if (ret != ReturnStatus::NoError) {
    processOperationError("echoServer", ret);
    return;
  }

  completeOperation("echoServer");
}

void ProductionManager::requestBox() {
  initOperation("requestBox");

  ReturnStatus ret;

  ret = Server->requestBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("requestBox", ret);
    return;
  }

  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    BoxData.clear();
    emit displayBoxData_signal(BoxData);
    processOperationError("requestBox", ret);
    return;
  }

  emit displayBoxData_signal(BoxData);

  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    TransponderData.clear();
    emit displayTransponderData_signal(TransponderData);
    processOperationError("requestBox", ret);
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  completeOperation("requestBox");
}

void ProductionManager::getCurrentBoxData() {
  initOperation("getCurrentBoxData");

  ReturnStatus ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    BoxData.clear();
    emit displayBoxData_signal(BoxData);
    processOperationError("getCurrentBoxData", ret);
    sendLog("Не удалось получить данные текущего бокса. ");
    return;
  }

  emit displayBoxData_signal(BoxData);

  completeOperation("getCurrentBoxData");
}

void ProductionManager::refundCurrentBox() {
  initOperation("refundCurrentBox");

  ReturnStatus ret;
  ret = Server->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("refundCurrentBox", ret);
    return;
  }

  BoxData.clear();
  emit displayBoxData_signal(BoxData);

  TransponderData.clear();
  emit displayTransponderData_signal(TransponderData);

  completeOperation("refundCurrentBox");
}

void ProductionManager::completeCurrentBox() {
  initOperation("completeCurrentBox");

  ReturnStatus ret;
  ret = Server->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    processOperationError("completeCurrentBox", ret);
    return;
  }

  BoxData.clear();
  emit displayBoxData_signal(BoxData);

  completeOperation("completeCurrentBox");
}

void ProductionManager::releaseTransponder() {
  initOperation("releaseTransponder");

  ReturnStatus ret;
  QString ucid;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME);
  StringDictionary result;
  StringDictionary param;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog("Flash-память разблокирована.");

  // Считываем UCID
  ret = Programmer->readUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Считанный UCID: %1.").arg(ucid));

  // Выпускаем транспондер
  ret = Server->releaseTransponder(result);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Транспондер выпущен."));

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    processOperationError("releaseTransponder", ReturnStatus::FileOpenError);
  }

  // Сохраняем прошивку в файл
  firmware.write(
      QByteArray::fromBase64(result.value("transponder_firmware").toUtf8()));
  firmware.close();
  sendLog(QString("Прошивка транспондера получена."));

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Прошивка загружена в микроконтроллер."));

  // Удаляем файл прошивки
  firmware.remove();

  // Подтверждаем выпуск транспондера
  param.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRelease(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Выпуск транспондера подтвержден."));

  // Обновляем данные бокса
  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    BoxData.clear();
    emit displayBoxData_signal(BoxData);
    processOperationError("getCurrentBoxData", ret);
    return;
  }

  emit displayBoxData_signal(BoxData);

  // Запрашиваем данные выпущенного транспондера
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    TransponderData.clear();
    emit displayTransponderData_signal(TransponderData);
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Данные выпускаемого транспондера получены."));

  emit displayTransponderData_signal(TransponderData);

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }
  sendLog(QString("Стикер распечатан."));

  completeOperation("releaseTransponder");
}

void ProductionManager::rereleaseTransponder(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("rereleaseTransponder");
  sendLog("Выпуск транспондера. ");

  ReturnStatus ret;
  QString ucid;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);
  StringDictionary result;
  StringDictionary requestParam;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  // Считываем UCID
  ret = Programmer->readUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  // Перевыпускаем транспондер
  requestParam.insert("transponder_pan", param->value("transponder_pan"));
  ret = Server->rereleaseTransponder(requestParam, result);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    sendLog("Не удалось сохранить файл прошивки. ");
    processOperationError("rereleaseTransponder", ReturnStatus::FileOpenError);
  }

  // Сохраняем прошивку в файл
  firmware.write(QByteArray::fromBase64(result.value("firmware").toUtf8()));
  firmware.close();

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  // Удаляем файл прошивки
  firmware.remove();

  // Подтверждаем перевыпуск транспондера
  requestParam.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRerelease(requestParam);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  // Запрашиваем данные перевыпущенного транспондера
  requestParam.remove("transponder_ucid");
  ret = Server->getTransponderData(requestParam, TransponderData);
  if (ret != ReturnStatus::NoError) {
    processOperationError("releaseTransponder", ret);
    return;
  }

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(TransponderData);

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
  if (ret != ReturnStatus::NoError) {
    processOperationError("rereleaseTransponder", ret);
    return;
  }

  completeOperation("rereleaseTransponder");
}

void ProductionManager::rollbackTransponder() {
  initOperation("rollbackTransponder");
  sendLog("Откат производственной линии. ");

  ReturnStatus ret;
  ret = Server->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    processOperationError("rollbackTransponder", ret);
    return;
  }

  // Обновляем данные бокса
  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    BoxData.clear();
    emit displayBoxData_signal(BoxData);
    processOperationError("getCurrentBoxData", ret);
    return;
  }

  emit displayBoxData_signal(BoxData);

  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    TransponderData.clear();
    emit displayTransponderData_signal(TransponderData);
    processOperationError("getCurrentTransponderData", ret);
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  completeOperation("rollbackTransponder");
}

void ProductionManager::getCurrentTransponderData() {
  initOperation("getCurrentTransponderData");

  ReturnStatus ret;
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    TransponderData.clear();
    emit displayTransponderData_signal(TransponderData);
    processOperationError("getCurrentTransponderData", ret);
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  completeOperation("getCurrentTransponderData");
}

void ProductionManager::getTransponderData(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("getTransponderData");

  ReturnStatus ret;
  ret = Server->getTransponderData(*param.get(), TransponderData);
  if (ret != ReturnStatus::NoError) {
    TransponderData.clear();
    emit displayTransponderData_signal(TransponderData);
    processOperationError("getTransponderData", ret);
    return;
  }

  emit displayTransponderData_signal(TransponderData);

  completeOperation("getTransponderData");
}

void ProductionManager::printBoxSticker(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("printBoxSticker");

  ReturnStatus ret;
  ret = Server->printBoxSticker(*param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printBoxSticker", ret);
    return;
  }

  completeOperation("printBoxSticker");
}

void ProductionManager::printLastBoxSticker() {
  initOperation("printLastBoxSticker");

  ReturnStatus ret;
  ret = Server->printLastBoxSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastBoxSticker", ret);
    return;
  }

  completeOperation("printLastBoxSticker");
}

void ProductionManager::printPalletSticker(
    const std::shared_ptr<StringDictionary> param) {
  initOperation("printPalletSticker");

  ReturnStatus ret;
  ret = Server->printPalletSticker(*param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printPalletSticker", ret);
    return;
  }

  completeOperation("printPalletSticker");
}

void ProductionManager::printLastPalletSticker() {
  initOperation("printLastPalletSticker");

  ReturnStatus ret;
  ret = Server->printLastPalletSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastPalletSticker", ret);
    return;
  }

  completeOperation("printLastPalletSticker");
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

void ProductionManager::createServerConnection() {
  Server = std::unique_ptr<AbstractServerConnection>(
      new PersoServerConnection("PersoServerConnection"));
}

void ProductionManager::createStickerPrinter() {
  StickerPrinter =
      std::unique_ptr<AbstractStickerPrinter>(new TE310Printer("TSC TE310"));

  if (!StickerPrinter->init()) {
    sendLog("Не удалось инициализировать принтер стикеров.");
  }
}

void ProductionManager::initOperation(const QString& name) {
  sendLog(QString("Начало выполнения операции %1. ").arg(name));
  emit executionStarted(name);
}

void ProductionManager::processOperationError(const QString& name,
                                              ReturnStatus ret) {
  sendLog(QString("Не удалось выполнить операцию %1. ").arg(name));
  emit executionFinished(name, ret);
}

void ProductionManager::completeOperation(const QString& name) {
  sendLog(QString("Операция %1 успешно выполнена. ").arg(name));
  emit executionFinished(name, ReturnStatus::NoError);
}
