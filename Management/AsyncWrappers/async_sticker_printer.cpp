#include "async_sticker_printer.h"

AsyncStickerPrinter::AsyncStickerPrinter(const QString& name,
                                         std::shared_ptr<IStickerPrinter> isp)
    : ProgressableAsyncWrapper(name), StickerPrinter(isp) {}

void AsyncStickerPrinter::printTransponderSticker(
    const StringDictionary& param) {
  initOperation("printTransponderSticker");

  ReturnStatus ret = StickerPrinter->printTransponderSticker(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printBoxSticker", ret);
    return;
  }

  completeOperation("printTransponderSticker");
}

void AsyncStickerPrinter::printLastTransponderSticker() {
  initOperation("printLastTransponderSticker");

  ReturnStatus ret = StickerPrinter->printLastTransponderSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastTransponderSticker", ret);
    return;
  }

  completeOperation("printLastTransponderSticker");
}

void AsyncStickerPrinter::printBoxSticker(const StringDictionary& param) {
  initOperation("printBoxSticker");

  ReturnStatus ret = StickerPrinter->printBoxSticker(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printBoxSticker", ret);
    return;
  }

  completeOperation("printBoxSticker");
}

void AsyncStickerPrinter::printLastBoxSticker() {
  initOperation("printLastBoxSticker");

  ReturnStatus ret = StickerPrinter->printLastBoxSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastBoxSticker", ret);
    return;
  }

  completeOperation("printLastBoxSticker");
}

void AsyncStickerPrinter::printPalletSticker(const StringDictionary& param) {
  initOperation("printPalletSticker");

  ReturnStatus ret = StickerPrinter->printPalletSticker(param);
  if (ret != ReturnStatus::NoError) {
    processOperationError("printPalletSticker", ret);
    return;
  }

  completeOperation("printPalletSticker");
}

void AsyncStickerPrinter::printLastPalletSticker() {
  initOperation("printLastPalletSticker");

  ReturnStatus ret = StickerPrinter->printLastPalletSticker();
  if (ret != ReturnStatus::NoError) {
    processOperationError("printLastPalletSticker", ret);
    return;
  }

  completeOperation("printLastPalletSticker");
}

void AsyncStickerPrinter::exec(const QStringList& script) {
  initOperation("exec");

  ReturnStatus ret = StickerPrinter->exec(script);
  if (ret != ReturnStatus::NoError) {
    processOperationError("execPrinterStickerCommandScript", ret);
    return;
  }

  completeOperation("exec");
}
