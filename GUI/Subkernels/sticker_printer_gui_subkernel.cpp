#include "sticker_printer_gui_subkernel.h"
#include "async_sticker_printer.h"
#include "global_environment.h"
#include "transponder_sticker_setup_dialog.h"

StickerPrinterGuiSubkernel::StickerPrinterGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
  connectDependecies();
}

void StickerPrinterGuiSubkernel::printLastTransponderSticker() {
  emit clearLogDisplay_signal();

  emit printLastTransponderSticker_signal();
}

void StickerPrinterGuiSubkernel::printTransponderSticker() {
  emit clearLogDisplay_signal();

  StringDictionary param;
  TransponderStickerSetupDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit printTransponderSticker_signal(param);
}

void StickerPrinterGuiSubkernel::exec(const QStringList& script) {
  emit clearLogDisplay_signal();

  emit exec_signal(script);
}

void StickerPrinterGuiSubkernel::connectDependecies() {
  AsyncStickerPrinter* asp =
      GlobalEnvironment::instance()->getObject<AsyncStickerPrinter>(
          "AsyncStickerPrinter");

  connect(this, &StickerPrinterGuiSubkernel::printLastTransponderSticker_signal,
          asp, &AsyncStickerPrinter::printLastTransponderSticker);
  connect(this, &StickerPrinterGuiSubkernel::printTransponderSticker_signal,
          asp, &AsyncStickerPrinter::printTransponderSticker);
  connect(this, &StickerPrinterGuiSubkernel::exec_signal, asp,
          &AsyncStickerPrinter::exec);
}
