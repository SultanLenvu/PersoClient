#include "production_tester_gui.h"

ProductionTesterGui::ProductionTesterGui(QWidget* parent)
    : AbstractGui(parent) {
  ControlPanelLayout = new QVBoxLayout();
  MainLayout->addLayout(ControlPanelLayout);

  TransponderGroup = new QGroupBox("Сборка");
  ControlPanelLayout->addWidget(TransponderGroup);

  TransponderGroupLayout = new QVBoxLayout();
  TransponderGroup->setLayout(TransponderGroupLayout);

  RereleaseTransponderButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  RereleaseTransponderButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderGroupLayout->addWidget(RereleaseTransponderButton);

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ControlPanelVS);

  PrinterStickerGroup = new QGroupBox("Печать");
  ControlPanelLayout->addWidget(PrinterStickerGroup);

  PrinterStickerLayout = new QVBoxLayout();
  PrinterStickerGroup->setLayout(PrinterStickerLayout);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  PrintLastTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintLastTransponderStickerButton);

  PrintCustomTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  PrintCustomTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintCustomTransponderStickerButton);

  PrintBoxStickerButton =
      new QPushButton(QString("Распечатать стикер для бокса"));
  PrintBoxStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintBoxStickerButton);

  PrintPalletStickerButton =
      new QPushButton(QString("Распечатать стикер для паллеты"));
  PrintPalletStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintPalletStickerButton);

  createModelViews();

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

void ProductionTesterGui::createModelViews() {
  ModelViewLayout = new QVBoxLayout();
  MainLayout->addLayout(ModelViewLayout);

  // Данные производственной линии
  ProductionLineDataGroup = new QGroupBox("Данные производственной линии");
  ModelViewLayout->addWidget(ProductionLineDataGroup);

  ProductionLineDataLayout = new QVBoxLayout();
  ProductionLineDataGroup->setLayout(ProductionLineDataLayout);

  ProductionLineDataView = new QTableView();
  ProductionLineDataLayout->addWidget(ProductionLineDataView);

  // Данные транспондера
  TransponderDataGroup = new QGroupBox("Данные перевыпущенного транспондера");
  ModelViewLayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);
}
