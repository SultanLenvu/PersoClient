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
  QFile firmware(
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME));
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

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    return ReturnStatus::FileOpenError;
  }

  // Сохраняем прошивку в файл
  firmware.write(
      QByteArray::fromBase64(result.value("transponder_firmware").toUtf8()));
  firmware.close();
  sendLog(QString("Прошивка транспондера получена."));

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }
  sendLog(QString("Прошивка загружена в микроконтроллер."));

  // Удаляем файл прошивки
  firmware.remove();

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

  ReturnStatus ret;
  QString ucid;
  QFile firmware(
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME));
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

  // Сохраняем присланный файл прошивки
  if (!firmware.open(QIODevice::WriteOnly)) {
    sendLog("Не удалось сохранить файл прошивки. ");
    return ReturnStatus::FileOpenError;
  }

  // Сохраняем прошивку в файл
  firmware.write(
      QByteArray::fromBase64(result.value("transponder_firmware").toUtf8()));
  firmware.close();

  // Загружаем прошивку
  ret = Programmer->programMemory(firmware);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  // Удаляем файл прошивки
  firmware.remove();

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
}
