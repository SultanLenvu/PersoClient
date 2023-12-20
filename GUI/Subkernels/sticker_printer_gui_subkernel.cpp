#include "sticker_printer_gui_subkernel.h"
#include "custom_transponder_sticker_dialog.h"
#include "master_gui.h"
#include "production_gui.h"
#include "testing_gui.h"

StickerPrinterGuiSubkernel::StickerPrinterGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

StickerPrinterGuiSubkernel::~StickerPrinterGuiSubkernel() {}

void StickerPrinterGuiSubkernel::setCurrentGui(
    std::shared_ptr<AbstractGui> gui) {
  CurrentGui = gui;

  switch (gui->type()) {
    case AbstractGui::Production:
      connectProductionGui();
      break;
    case AbstractGui::Testing:
      connectTestingGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    default:
      break;
  }
}

void StickerPrinterGuiSubkernel::connectProductionGui() {
  ProductionGUI* gui = dynamic_cast<ProductionGUI*>(CurrentGui.get());

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::connectTestingGui() {
  TestingGui* gui = dynamic_cast<TestingGui*>(CurrentGui.get());

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
}

void StickerPrinterGuiSubkernel::connectMasterGui() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui.get());

  connect(gui->PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker_guiSlot);
  connect(gui->PrintCustomTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterGuiSubkernel::printCustomTransponderSticker_guiSlot);
  connect(
      gui->ExecuteStickerPrinterCommandScriptButton, &QPushButton::clicked,
      this,
      &StickerPrinterGuiSubkernel::executeStickerPrinterCommandScript_guiSlot);
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

  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui);
  std::shared_ptr<QStringList> script(new QStringList(
      gui->StickerPrinterCommandSriptTextEdit->toPlainText().split("\n\r")));
  emit executeCommandScript_signal(script);
}
