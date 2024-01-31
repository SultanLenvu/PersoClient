#include "sticker_printer_manager.h"
#include "te310_printer.h"

StickerPrinterManager::StickerPrinterManager(const QString& name)
    : AbstractManager{name} {
  loadSettings();
}

StickerPrinterManager::~StickerPrinterManager() {}

void StickerPrinterManager::onInstanceThreadStarted() {
  createStickerPrinter();
}

AbstractManager::Type StickerPrinterManager::type() const {
  return Type::StickerPrinter;
}

void StickerPrinterManager::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();

  StickerPrinter->applySetting();
}

void StickerPrinterManager::printTransponderSticker(
    std::shared_ptr<StringDictionary> param) {
  emit executionStarted("printTransponderSticker");
  sendLog("Печать стикера для транспондера. ");

  ReturnStatus ret;
  ret = StickerPrinter->printTransponderSticker(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printTransponderSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Cтикер для транспондера успешно распечатан. ");
  emit executionFinished("printTransponderSticker", ret);
}

void StickerPrinterManager::printLastTransponderSticker() {
  emit executionStarted("printLastTransponderSticker");
  sendLog("Печать последнего стикера для транспондера. ");

  ReturnStatus ret;
  ret = StickerPrinter->printLastTransponderSticker();
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printLastTransponderSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Последний стикер для транспондера успешно распечатан. ");
  emit executionFinished("printLastTransponderSticker", ret);
}

void StickerPrinterManager::printCustomTransponderSticker(
    std::shared_ptr<StringDictionary> param) {
  emit executionStarted("printCustomTransponderSticker");
  sendLog("Печать произвольного стикера для транспондера. ");

  ReturnStatus ret;
  ret = StickerPrinter->printTransponderSticker(*param);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("printCustomTransponderSticker", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Стикер для транспондера успешно распечатан. ");
  emit executionFinished("printCustomTransponderSticker", ret);
}

void StickerPrinterManager::executeCommandScript(
    std::shared_ptr<QStringList> script) {
  emit executionStarted("executeCommandScript");
  sendLog("Выполнение командного скрипта для принтера. ");

  ReturnStatus ret;
  ret = StickerPrinter->exec(*script);
  if (ret != ReturnStatus::NoError) {
    emit executionFinished("executeCommandScript", ret);
    return;
  }

  // Завершаем операцию
  sendLog("Командный скрипт успешно выполнен. ");
  emit executionFinished("executeCommandScript", ret);
}

void StickerPrinterManager::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

void StickerPrinterManager::loadSettings() {}

void StickerPrinterManager::createStickerPrinter() {
  StickerPrinter = std::unique_ptr<AbstractStickerPrinter>(
      new TE310Printer("StickerPrinter2"));
}
