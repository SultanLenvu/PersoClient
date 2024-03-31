#include <QDir>

#include "definitions.h"
#include "production_manager.h"

ProductionManager::ProductionManager(
    const QString& name,
    std::shared_ptr<IServerConnection> server,
    std::shared_ptr<IStickerPrinter> stickerPrinter,
    std::shared_ptr<IProgrammer> programmer)
    : NamedObject{name},
      LoggableObject(name),
      Server(server),
      StickerPrinter(stickerPrinter) {}

StringDictionary& ProductionManager::productionLineData() {
  return ProductionLineData;
}

StringDictionary& ProductionManager::boxData() {
  return BoxData;
}

StringDictionary& ProductionManager::transponderData() {
  return TransponderData;
}

ReturnStatus ProductionManager::logOn(const StringDictionary& param) {
  ReturnStatus ret = ReturnStatus::NoError;

  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->launchProductionLine(param);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getProductionLineData(ProductionLineData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::logOut() {
  if (Server->isConnected()) {
    Server->shutdownProductionLine();
    Server->disconnect();
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::requestBox() {
  ReturnStatus ret = ReturnStatus::NoError;

  ret = Server->requestBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getProductionLineData(ProductionLineData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Если в боксе есть собранные транспондеры
  if (BoxData.value("box_assembled_units").toInt() > 0) {
    ret = Server->getCurrentTransponderData(TransponderData);
    if (ret != ReturnStatus::NoError) {
      return ret;
    }
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::refundCurrentBox() {
  ReturnStatus ret = ReturnStatus::NoError;
  ret = Server->refundCurrentBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  BoxData.clear();

  ret = Server->getProductionLineData(ProductionLineData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::completeCurrentBox() {
  ReturnStatus ret = ReturnStatus::NoError;
  ret = Server->completeCurrentBox();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  BoxData.clear();

  ret = Server->getProductionLineData(ProductionLineData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::releaseTransponder() {
  ReturnStatus ret = ReturnStatus::NoError;
  QString ucid;
  QByteArray firmware;
  StringDictionary result;
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
  ret = Server->releaseTransponder(result);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Транспондер выпущен."));

  firmware =
      QByteArray::fromBase64(result.value("transponder_firmware").toUtf8());
  // Проверка корректности присланной прошивки
  if (firmware.size() != TRANSPONDER_FIRMWARE_SIZE) {
    sendLog(QString("Получен некорректный файл прошивки. "));
    return ReturnStatus::InvalidFirmwareFile;
  }
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
  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Запрашиваем данные выпущенного транспондера
  ret = Server->getCurrentTransponderData(TransponderData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Данные выпускаемого транспондера получены."));

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
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
  StringDictionary result;
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
  ret = Server->rereleaseTransponder(requestParam, result);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  firmware =
      QByteArray::fromBase64(result.value("transponder_firmware").toUtf8());
  // Проверка корректности присланной прошивки
  if (firmware.size() != TRANSPONDER_FIRMWARE_SIZE) {
    sendLog(QString("Получен некорректный файл прошивки. "));
    return ReturnStatus::InvalidFirmwareFile;
  }
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
  ret = Server->getTransponderData(requestParam, TransponderData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Печатаем стикер
  ret = StickerPrinter->printTransponderSticker(TransponderData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus ProductionManager::rollbackTransponder() {
  sendLog("Откат производственной линии. ");

  ReturnStatus ret = ReturnStatus::NoError;
  ret = Server->rollbackTransponder();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Обновляем данные бокса
  ret = Server->getCurrentBoxData(BoxData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }


  // Если в боксе есть собранные транспондеры
  if (BoxData.value("box_assembled_units").toInt() > 0) {
    ret = Server->getCurrentTransponderData(TransponderData);
    if (ret != ReturnStatus::NoError) {
      return ret;
    }
  }

  return ReturnStatus::NoError;
}
