#include <QDir>

#include "definitions.h"

#include "production_manager.h"

ProductionManager::ProductionManager(
    std::shared_ptr<ProductionUnitContext> context,
    std::shared_ptr<IServerConnection> server,
    std::shared_ptr<IStickerPrinter> stickerPrinter,
    std::shared_ptr<IProgrammer> programmer)
    : LoggableObject("ProductionManager"),
      Context(context),
      Server(server),
      StickerPrinter(stickerPrinter),
      Programmer(programmer) {}

ReturnStatus ProductionManager::logOn(const StringDictionary& param) {
  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary state;

  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->launchProductionLine(param);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getProductionLineData(state);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setState(state);

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::logOut() {
  if (Server->isConnected()) {
    Server->shutdownProductionLine();
    Server->disconnect();
  }
  Context->clear();

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::requestBox() {
  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary box;
  StringDictionary state;
  StringDictionary transponder;

  ret = Server->requestBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getCurrentBoxData(box);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setBox(box);

  ret = Server->getProductionLineData(state);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setState(state);

  // Если в боксе есть собранные транспондеры
  if (!Context->isBoxEmpty()) {
    ret = Server->getCurrentTransponderData(transponder);
    if (ret != ReturnStatus::NoError) {
      return ret;
    }
  }
  Context->setTransponder(transponder);

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::refundCurrentBox() {
  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary state;

  ret = Server->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->refundBox();

  ret = Server->getProductionLineData(state);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setState(state);

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::completeCurrentBox() {
  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary state;

  ret = Server->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->completeBox();

  ret = Server->getProductionLineData(state);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setState(state);

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::releaseTransponder() {
  ReturnStatus ret = ReturnStatus::NoError;
  QString ucid;
  QByteArray firmware;
  StringDictionary data;
  StringDictionary box;
  StringDictionary transponder;
  StringDictionary param;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog("Flash-память разблокирована.");

  // Считываем UCID
  ret = Programmer->readTransponderUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Считанный UCID: %1.").arg(ucid));

  // Выпускаем транспондер
  ret = Server->releaseTransponder(data);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Транспондер выпущен."));

  firmware =
      QByteArray::fromBase64(data.value("transponder_firmware").toUtf8());
  // Проверка корректности присланной прошивки
  if (firmware.size() != TRANSPONDER_FIRMWARE_SIZE) {
    sendLog(QString("Получен некорректный файл прошивки. "));
    return ReturnStatus::InvalidFirmwareFile;
  }
  Context->setFirmware(firmware.toHex());
  sendLog(QString("Прошивка транспондера получена."));

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Прошивка загружена в микроконтроллер."));

  // Подтверждаем выпуск транспондера
  param.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRelease(param);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Выпуск транспондера подтвержден."));

  // Обновляем данные бокса
  ret = Server->getCurrentBoxData(box);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setBox(box);

  // Запрашиваем данные выпущенного транспондера
  ret = Server->getCurrentTransponderData(transponder);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setTransponder(transponder);
  sendLog(QString("Данные выпускаемого транспондера получены."));

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(transponder);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Стикер распечатан."));

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::rereleaseTransponder(
    const StringDictionary& param) {
  sendLog("Выпуск транспондера. ");

  ReturnStatus ret = ReturnStatus::NoError;
  QString ucid;
  QByteArray firmware;
  StringDictionary data;
  StringDictionary transponder;
  StringDictionary requestParam;

  // Разблокируем память
  ret = Programmer->unlockMemory();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Считываем UCID
  ret = Programmer->readTransponderUcid(ucid);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Перевыпускаем транспондер
  requestParam.insert("transponder_pan", param.value("transponder_pan"));
  ret = Server->rereleaseTransponder(requestParam, data);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  firmware =
      QByteArray::fromBase64(data.value("transponder_firmware").toUtf8());
  // Проверка корректности присланной прошивки
  if (firmware.size() != TRANSPONDER_FIRMWARE_SIZE) {
    sendLog(QString("Получен некорректный файл прошивки. "));
    return ReturnStatus::InvalidFirmwareFile;
  }
  data.clear();
  Context->setFirmware(firmware);
  sendLog(QString("Прошивка транспондера получена."));

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Подтверждаем перевыпуск транспондера
  requestParam.insert("transponder_ucid", ucid);
  ret = Server->confirmTransponderRerelease(requestParam);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Запрашиваем данные перевыпущенного транспондера
  requestParam.remove("transponder_ucid");
  ret = Server->getTransponderData(requestParam, transponder);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setTransponder(transponder);

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(transponder);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::rollbackTransponder() {
  sendLog("Откат производственной линии. ");

  ReturnStatus ret = ReturnStatus::NoError;
  StringDictionary transponder;
  StringDictionary box;

  ret = Server->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Обновляем данные бокса
  ret = Server->getCurrentBoxData(box);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  Context->setBox(box);

  // Если в боксе есть собранные транспондеры
  if (!Context->isBoxEmpty()) {
    ret = Server->getCurrentTransponderData(transponder);
    if (ret != ReturnStatus::NoError) {
      return ret;
    }
  }
  Context->setTransponder(transponder);

  return ReturnStatus::NoError;
}
