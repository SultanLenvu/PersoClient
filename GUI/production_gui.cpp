#include "production_gui.h"

ProductionGUI::ProductionGUI(QWidget* parent)
    : AbstractGui(parent, Production) {
  ControlPanelLayout = new QVBoxLayout();
  MainLayout->addLayout(ControlPanelLayout);

  TransponderGroup = new QGroupBox("Сборка");
  ControlPanelLayout->addWidget(TransponderGroup);

  TransponderLayout = new QVBoxLayout();
  TransponderGroup->setLayout(TransponderLayout);

  LoadTransponderFirmwareButton =
      new QPushButton(QString("Выпустить транспондер"));
  LoadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderLayout->addWidget(LoadTransponderFirmwareButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  TransponderLayout->addItem(ButtonVerticalSpacer);

  RollbackProductionLinePushButton =
      new QPushButton(QString("Произвести откат"));
  RollbackProductionLinePushButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderLayout->addWidget(RollbackProductionLinePushButton);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderLayout->addWidget(ReloadTransponderFirmwareButton);

  PrintStickerGroup = new QGroupBox("Печать");
  ControlPanelLayout->addWidget(PrintStickerGroup);

  PrintStickerLayout = new QVBoxLayout();
  PrintStickerGroup->setLayout(PrintStickerLayout);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  PrintLastTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrintStickerLayout->addWidget(PrintLastTransponderStickerButton);

  PrintCustomTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  PrintCustomTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrintStickerLayout->addWidget(PrintCustomTransponderStickerButton);

  PrintBoxStickerButton =
      new QPushButton(QString("Распечатать стикер для бокса"));
  PrintBoxStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrintStickerLayout->addWidget(PrintBoxStickerButton);

  TransponderDataGroup = new QGroupBox("Данные транспондера");
  MainLayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

ProductionGUI::~ProductionGUI() {}

void ProductionGUI::update() {
  TransponderDataView->resizeColumnsToContents();
  TransponderDataView->update();
}
