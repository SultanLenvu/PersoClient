#include "production_gui.h"

ProductionGUI::ProductionGUI(QWidget* parent)
    : AbstractGUI(parent, Production) {
  ControlPanelGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(ControlPanelGroup);

  ControlPanelLayout = new QVBoxLayout();
  ControlPanelGroup->setLayout(ControlPanelLayout);

  LoadTransponderFirmwareButton =
      new QPushButton(QString("Выпустить транспондер"));
  LoadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(LoadTransponderFirmwareButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVerticalSpacer);

  RollbackProductionLinePushButton =
      new QPushButton(QString("Произвести откат"));
  RollbackProductionLinePushButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(RollbackProductionLinePushButton);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  PrintLastTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(PrintLastTransponderStickerButton);

  PrintCustomTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  PrintCustomTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(PrintCustomTransponderStickerButton);

  TransponderDataGroup = new QGroupBox("Данные о выпущенном транспондере");
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
