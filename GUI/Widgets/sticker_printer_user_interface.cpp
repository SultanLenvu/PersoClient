#include "sticker_printer_user_interface.h"
#include "global_environment.h"
#include "sticker_printer_gui_subkernel.h"

StickerPrinterUserInterface::StickerPrinterUserInterface(QWidget* parent)
    : QWidget{parent} {
  create();
  connectDependencies();
}

void StickerPrinterUserInterface::create() {
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
          &StickerPrinterUserInterface::execCommandSript);

  CommandSriptGroup = new QGroupBox(QString("Командный скрипт"));
  MainLayout->addWidget(CommandSriptGroup);
  CommandSriptLayout = new QVBoxLayout();
  CommandSriptGroup->setLayout(CommandSriptLayout);
  CommandSriptInput = new QPlainTextEdit();
  CommandSriptLayout->addWidget(CommandSriptInput);
}

void StickerPrinterUserInterface::connectDependencies() {
  StickerPrinterGuiSubkernel* spgs =
      GlobalEnvironment::instance()->getObject<StickerPrinterGuiSubkernel>(
          "StickerPrinterGuiSubkernel");

  connect(PrintTransponderStickerButton, &QPushButton::clicked, spgs,
          &StickerPrinterGuiSubkernel::printTransponderSticker);
  connect(PrintLastTransponderStickerButton, &QPushButton::clicked, spgs,
          &StickerPrinterGuiSubkernel::printLastTransponderSticker);

  connect(this, &StickerPrinterUserInterface::execCommandScript_signal, spgs,
          &StickerPrinterGuiSubkernel::exec);
}

void StickerPrinterUserInterface::execCommandSript() {
  QString scriptStr = CommandSriptInput->toPlainText();
  scriptStr.remove("\r");

  QStringList script = scriptStr.split("\n");
  emit execCommandScript_signal(script);
}
