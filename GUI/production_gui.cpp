#include "production_gui.h"

ProductionGUI::ProductionGUI(QWidget* parent) : GUI(parent, Production) {}

void ProductionGUI::create(void) {
  ControlPanelGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(ControlPanelGroup);

  ControlPanelLayout = new QVBoxLayout();
  ControlPanelGroup->setLayout(ControlPanelLayout);

  LoadTransponderFirmwareButton =
      new QPushButton(QString("Выпуск транспондера"));
  LoadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(LoadTransponderFirmwareButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVerticalSpacer);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпуск транспондера"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  TransponderSeedGroup = new QGroupBox("Данные о выпущенном транспондере");
  MainLayout->addWidget(TransponderSeedGroup);

  TransponderSeedLayout = new QVBoxLayout();
  TransponderSeedGroup->setLayout(TransponderSeedLayout);

  TransponderSeedView = new QTableView();
  TransponderSeedLayout->addWidget(TransponderSeedView);

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

void ProductionGUI::update() {
  TransponderSeedView->resizeColumnsToContents();
}
