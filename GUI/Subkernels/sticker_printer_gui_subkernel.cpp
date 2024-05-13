#include "sticker_printer_gui_subkernel.h"
#include "custom_transponder_sticker_dialog.h"
#include "master_gui.h"
#include "production_assembler_gui.h"
#include "production_tester_gui.h"
#include "sticker_printer_manager.h"

StickerPrinterGuiSubkernel::StickerPrinterGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

StickerPrinterGuiSubkernel::~StickerPrinterGuiSubkernel() {}

void StickerPrinterGuiSubkernel::connectGui(AbstractGui* gui) {
  CurrentGui = gui;

  switch (CurrentGui->type()) {
    case AbstractGui::Authorization:
      connectAuthorizationGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    case AbstractGui::ProductionAssembler:
      connectProductionAssemblerGui();
      break;
    case AbstractGui::ProductionTester:
      connectProductionTesterGui();
      break;
    default:
      break;
  }
}

void StickerPrinterGuiSubkernel::connectManager(
    std::shared_ptr<AbstractManager> manager) {
  if (manager->type() != AbstractManager::StickerPrinter) {
    return;
  }

  Manager = manager;
  connectStickerPrinterManager();
}

void StickerPrinterGuiSubkernel::reset() {
  Manager.reset();
}

void StickerPrinterGuiSubkernel::connectAuthorizationGui() {}

void StickerPrinterGuiSubkernel::connectMasterGui() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui);

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
  connect(
      gui->ExecuteStickerPrinterCommandScriptButton, &QPushButton::clicked,
      this,
      &StickerPrinterGuiSubkernel::executeStickerPrinterCommandScript_guiSlot);
}

void StickerPrinterGuiSubkernel::connectProductionAssemblerGui() {
  ProductionAssemblerGui* gui =
      dynamic_cast<ProductionAssemblerGui*>(CurrentGui);

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::connectProductionTesterGui() {
  ProductionTesterGui* gui = dynamic_cast<ProductionTesterGui*>(CurrentGui);

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::connectStickerPrinterManager() const {
  StickerPrinterManager* manager =
      dynamic_cast<StickerPrinterManager*>(Manager.get());

  connect(this, &StickerPrinterGuiSubkernel::printLastTransponderSticker_signal,
          manager, &StickerPrinterManager::printLastTransponderSticker_async);
  connect(this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_signal,
          manager, &StickerPrinterManager::printCustomTransponderSticker_async);
  connect(this, &StickerPrinterGuiSubkernel::executeCommandScript_signal,
          manager, &StickerPrinterManager::executeCommandScript_async);
}

void StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot() {
  emit clearLogDisplay_signal();

  emit printLastTransponderSticker_signal();
}

void StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<StringDictionary> param(new StringDictionary());
  CustomTransponderStickerScanDialog dialog(nullptr);
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(*param);

  emit printCustomTransponderSticker_signal(param);
}

void StickerPrinterGuiSubkernel::executeStickerPrinterCommandScript_guiSlot() {
  emit clearLogDisplay_signal();

  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui);
  std::shared_ptr<QStringList> script(new QStringList(
      gui->StickerPrinterCommandSriptTextEdit->toPlainText().split("\n\r")));
  emit executeCommandScript_signal(script);
}
