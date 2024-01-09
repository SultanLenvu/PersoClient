#include "sticker_printer_gui_subkernel.h"
#include "custom_transponder_sticker_dialog.h"
#include "master_gui.h"
#include "production_assembler_gui.h"
#include "production_tester_gui.h"

StickerPrinterGuiSubkernel::StickerPrinterGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

StickerPrinterGuiSubkernel::~StickerPrinterGuiSubkernel() {}

void StickerPrinterGuiSubkernel::connectAuthorizationGui(
    std::shared_ptr<AuthorizationGui> gui) {
  CurrentGui = gui;
}

void StickerPrinterGuiSubkernel::connectMasterGui(
    std::shared_ptr<MasterGui> gui) {
  CurrentGui = gui;

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
  connect(
      gui->ExecuteStickerPrinterCommandScriptButton, &QPushButton::clicked,
      this,
      &StickerPrinterGuiSubkernel::executeStickerPrinterCommandScript_guiSlot);
}

void StickerPrinterGuiSubkernel::connectProductionAssemblerGui(
    std::shared_ptr<ProductionAssemblerGui> gui) {
  CurrentGui = gui;

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::connectProductionTesterGui(
    std::shared_ptr<ProductionTesterGui> gui) {
  CurrentGui = gui;

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::resetCurrentGui() {
  CurrentGui.reset();
}

void StickerPrinterGuiSubkernel::connectManager(
    const StickerPrinterManager* manager) const {
  connect(this, &StickerPrinterGuiSubkernel::printLastTransponderSticker_signal,
          manager, &StickerPrinterManager::printLastTransponderSticker);
  connect(this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_signal,
          manager, &StickerPrinterManager::printCustomTransponderSticker);
  connect(this, &StickerPrinterGuiSubkernel::executeCommandScript_signal,
          manager, &StickerPrinterManager::executeCommandScript);
}

void StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot() {
  emit loggerClear_signal();

  emit printLastTransponderSticker_signal();
}

void StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  CustomTransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printCustomTransponderSticker_signal(param);
}

void StickerPrinterGuiSubkernel::executeStickerPrinterCommandScript_guiSlot() {
  emit loggerClear_signal();

  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui.get());
  std::shared_ptr<QStringList> script(new QStringList(
      gui->StickerPrinterCommandSriptTextEdit->toPlainText().split("\n\r")));
  emit executeCommandScript_signal(script);
}
