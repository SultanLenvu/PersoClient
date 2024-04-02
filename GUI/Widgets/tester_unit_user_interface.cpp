#include "tester_unit_user_interface.h"
#include "async_production_manager.h"
#include "async_server_connection.h"
#include "async_sticker_printer.h"
#include "global_environment.h"

TesterUnitUserInterface::TesterUnitUserInterface(QWidget* parent)
    : QWidget(parent) {
  create();
}

void TesterUnitUserInterface::connectDependencies() {
  AsyncProductionManager* apm =
      GlobalEnvironment::instance()->getObject<AsyncProductionManager>(
          "AsyncProductionManager");

  connect(RereleaseTransponderButton, &QPushButton::clicked, apm,
          &AsyncProductionManager::rereleaseTransponder);

  AsyncServerConnection* asc =
      GlobalEnvironment::instance()->getObject<AsyncServerConnection>(
          "AsyncServerConnection");

  connect(PrintLastBoxStickerButton, &QPushButton::clicked, asc,
          &AsyncServerConnection::printLastBoxSticker);
  connect(PrintBoxStickerButton, &QPushButton::clicked, asc,
          &AsyncServerConnection::printBoxSticker);
  connect(PrintLastPalletStickerButton, &QPushButton::clicked, asc,
          &AsyncServerConnection::printLastPalletSticker);
  connect(PrintPalletStickerButton, &QPushButton::clicked, asc,
          &AsyncServerConnection::printPalletSticker);

  AsyncStickerPrinter* asp =
      GlobalEnvironment::instance()->getObject<AsyncStickerPrinter>(
          "AsyncStickerPrinter");

  // Связывание моделей и представлений
  ProductionLineDataView->setModel(ProductionLineModel.get());
  TransponderDataView->setModel(TransponderDataModel.get());
}

void TesterUnitUserInterface::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  ControlPanelLayout = new QVBoxLayout();
  MainLayout->addLayout(ControlPanelLayout);

  createTransponderGroup();
  createPrinterStickerGroup();
  createModelViews();

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

void TesterUnitUserInterface::createTransponderGroup() {
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
}

void TesterUnitUserInterface::createPrinterStickerGroup() {
  PrinterStickerGroup = new QGroupBox("Печать");
  ControlPanelLayout->addWidget(PrinterStickerGroup);

  PrinterStickerLayout = new QVBoxLayout();
  PrinterStickerGroup->setLayout(PrinterStickerLayout);

  PrintLastTransponderStickerButton =
      new QPushButton(QString("Распечатать последний стикер"));
  PrintLastTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintLastTransponderStickerButton);

  PrintTransponderStickerButton =
      new QPushButton(QString("Распечатать произвольный стикер"));
  PrintTransponderStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintTransponderStickerButton);

  ControlPanelVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  PrinterStickerLayout->addItem(ControlPanelVS1);

  PrintLastBoxStickerButton =
      new QPushButton(QString("Распечатать последний стикер для бокса"));
  PrintLastBoxStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintLastBoxStickerButton);

  PrintBoxStickerButton =
      new QPushButton(QString("Распечатать стикер для бокса"));
  PrintBoxStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintBoxStickerButton);

  ControlPanelVS2 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  PrinterStickerLayout->addItem(ControlPanelVS2);

  PrintLastPalletStickerButton =
      new QPushButton(QString("Распечатать последний стикер для паллеты"));
  PrintLastPalletStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintLastPalletStickerButton);

  PrintPalletStickerButton =
      new QPushButton(QString("Распечатать стикер для паллеты"));
  PrintPalletStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintPalletStickerButton);
}

void TesterUnitUserInterface::createModelViews() {
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
