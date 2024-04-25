#include "sticker_printer_widget.h"
#include "transponder_sticker_setup_dialog.h"

StickerPrinterWidget::StickerPrinterWidget(QWidget* parent) : QWidget{parent} {
  create();
  connectInternals();
}

void StickerPrinterWidget::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  ControlPanel = new QGroupBox(QString("Панель управления"));
  MainLayout->addWidget(ControlPanel);
  ControlPanelLayout = new QVBoxLayout();
  ControlPanel->setLayout(ControlPanelLayout);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  ControlPanelLayout->addWidget(PrintLastTransponderStickerButton);
  PrintTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  ControlPanelLayout->addWidget(PrintTransponderStickerButton);
  ControlPanelVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ControlPanelVS1);
  ExecCommandScriptButton =
      new QPushButton(QString("Выполнить командный скрипт"));
  ControlPanelLayout->addWidget(ExecCommandScriptButton);
  connect(ExecCommandScriptButton, &QPushButton::clicked, this,
          &StickerPrinterWidget::execCommandSript);

  CommandSriptGroup = new QGroupBox(QString("Командный скрипт"));
  MainLayout->addWidget(CommandSriptGroup);
  CommandSriptLayout = new QVBoxLayout();
  CommandSriptGroup->setLayout(CommandSriptLayout);
  CommandSriptInput = new QPlainTextEdit();
  CommandSriptLayout->addWidget(CommandSriptInput);
}

void StickerPrinterWidget::connectInternals() {
  connect(PrintTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterWidget::printTransponderSticker);
  connect(ExecCommandScriptButton, &QPushButton::clicked, this,
          &StickerPrinterWidget::execCommandSript);

  connect(PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &StickerPrinterWidget::printLastTransponderSticker_trigger);
}

void StickerPrinterWidget::execCommandSript() {
  QString scriptStr = CommandSriptInput->toPlainText();
  scriptStr.remove("\r");

  QStringList script = scriptStr.split("\n");
  emit execCommandScript_trigger(script);
}

void StickerPrinterWidget::printTransponderSticker() {
  StringDictionary param;
  TransponderStickerSetupDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit printTransponderSticker_trigger(param);
}
