#include "production_gui.h"

ProductionGUI::ProductionGUI(QWidget* parent) : GUI(parent, Production) {}

void ProductionGUI::create(void) {
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

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  PrintLastTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(PrintLastTransponderStickerButton);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпустить транспондера"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  TransponderInfoGroup = new QGroupBox("Данные о выпущенном транспондере");
  MainLayout->addWidget(TransponderInfoGroup);

  TransponderInfoLayout = new QVBoxLayout();
  TransponderInfoGroup->setLayout(TransponderInfoLayout);

  TransponderInfoView = new QTableView();
  TransponderInfoLayout->addWidget(TransponderInfoView);

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

void ProductionGUI::update() {
  TransponderInfoView->resizeColumnsToContents();
  TransponderInfoView->update();
}
