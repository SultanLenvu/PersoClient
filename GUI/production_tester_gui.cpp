#include "production_tester_gui.h"

ProductionTesterGui::ProductionTesterGui(QWidget* parent)
    : AbstractGui(parent) {
  ControlPanelLayout = new QVBoxLayout();
  MainLayout->addLayout(ControlPanelLayout);

  TransponderGroup = new QGroupBox("Сборка");
  ControlPanelLayout->addWidget(TransponderGroup);

  TransponderLayout = new QVBoxLayout();
  TransponderGroup->setLayout(TransponderLayout);

  RereleaseTransponderButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  RereleaseTransponderButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderLayout->addWidget(RereleaseTransponderButton);

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ControlPanelVS);

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

  PrintPalletStickerButton =
      new QPushButton(QString("Распечатать стикер для паллеты"));
  PrintPalletStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrintStickerLayout->addWidget(PrintPalletStickerButton);

  TransponderDataGroup = new QGroupBox("Данные транспондера");
  MainLayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

ProductionTesterGui::~ProductionTesterGui() {}

void ProductionTesterGui::updateModelViews() {
  TransponderDataView->resizeColumnsToContents();
  TransponderDataView->update();
}

AbstractGui::GuiType ProductionTesterGui::type() {
  return ProductionTester;
}
