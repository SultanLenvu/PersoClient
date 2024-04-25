#include "sticker_printer_gui_subkernel.h"

#include "transponder_sticker_setup_dialog.h"

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
