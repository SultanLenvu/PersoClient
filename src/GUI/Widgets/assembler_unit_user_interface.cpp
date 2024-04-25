#include "assembler_unit_user_interface.h"

AssemblerUnitUserInterface::AssemblerUnitUserInterface(QWidget* parent)
    : QWidget(parent) {
  ControlPanelLayout = new QVBoxLayout();
  MainLayout->addLayout(ControlPanelLayout);

  createBoxGroup();
  createTransponderGroup();
  createPrinterStickerGroup();
  createModelViews();

  MainLayout->setStretch(0, 1);
  MainLayout->setStretch(1, 3);
}

void AssemblerUnitUserInterface::setStateModel(QAbstractItemModel* model) {
  ProductionLineDataView->setModel(model);
}

void AssemblerUnitUserInterface::setTransponderModel(
    QAbstractItemModel* model) {
  TransponderDataView->setModel(model);
}

void AssemblerUnitUserInterface::setBoxModel(QAbstractItemModel* model) {
  BoxDataView->setModel(model);
}

void AssemblerUnitUserInterface::connectInternals() {
  connect(RequestBoxButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::requestBox_trigger);
  connect(RefundCurrentBoxButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::refundCurrentBox_trigger);
  connect(CompleteCurrentBoxButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::completeCurrentBox_trigger);

  connect(ReleaseTransponderButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::releaseTransponder_trigger);
  connect(RereleaseTransponderButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::rereleaseTransponder_trigger);
  connect(RollbackTransponderPushButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::rollbackTransponder_trigger);

  connect(PrintLastTransponderStickerButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::printLastTransponderSticker_trigger);
  connect(PrintTransponderStickerButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::printTransponderSticker_trigger);
  connect(PrintBoxStickerButton, &QPushButton::clicked, this,
          &AssemblerUnitUserInterface::printBoxSticker_trigger);
}

void AssemblerUnitUserInterface::createBoxGroup() {
  BoxGroup = new QGroupBox("Бокс");
  ControlPanelLayout->addWidget(BoxGroup);

  BoxGroupLayout = new QVBoxLayout();
  BoxGroup->setLayout(BoxGroupLayout);

  RequestBoxButton = new QPushButton(QString("Запросить"));
  RequestBoxButton->setFont(QFont("Arial", 12, QFont::Bold));
  BoxGroupLayout->addWidget(RequestBoxButton);

  RefundCurrentBoxButton = new QPushButton(QString("Вернуть"));
  RefundCurrentBoxButton->setFont(QFont("Arial", 12, QFont::Bold));
  BoxGroupLayout->addWidget(RefundCurrentBoxButton);

  CompleteCurrentBoxButton = new QPushButton(QString("Завершить сборку"));
  CompleteCurrentBoxButton->setFont(QFont("Arial", 12, QFont::Bold));
  BoxGroupLayout->addWidget(CompleteCurrentBoxButton);

  ButtonVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVS1);
}

void AssemblerUnitUserInterface::createTransponderGroup() {
  TransponderGroup = new QGroupBox("Транспондер");
  ControlPanelLayout->addWidget(TransponderGroup);

  TransponderGroupLayout = new QVBoxLayout();
  TransponderGroup->setLayout(TransponderGroupLayout);

  ReleaseTransponderButton = new QPushButton(QString("Выпустить"));
  ReleaseTransponderButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderGroupLayout->addWidget(ReleaseTransponderButton);

  ButtonVS2 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  TransponderGroupLayout->addItem(ButtonVS2);

  RereleaseTransponderButton = new QPushButton(QString("Перевыпустить"));
  RereleaseTransponderButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderGroupLayout->addWidget(RereleaseTransponderButton);

  RollbackTransponderPushButton = new QPushButton(QString("Откатить"));
  RollbackTransponderPushButton->setFont(QFont("Arial", 12, QFont::Bold));
  TransponderGroupLayout->addWidget(RollbackTransponderPushButton);

  ButtonVS3 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVS3);
}

void AssemblerUnitUserInterface::createPrinterStickerGroup() {
  PrinterStickerGroup = new QGroupBox("Принтер стикеров");
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

  PrintBoxStickerButton =
      new QPushButton(QString("Распечатать стикер для бокса"));
  PrintBoxStickerButton->setFont(QFont("Arial", 12, QFont::Bold));
  PrinterStickerLayout->addWidget(PrintBoxStickerButton);
}

void AssemblerUnitUserInterface::createModelViews() {
  ModelViewLayout = new QVBoxLayout();
  MainLayout->addLayout(ModelViewLayout);

  // Данные производственной линии
  ProductionLineDataGroup = new QGroupBox("Данные производственной линии");
  ModelViewLayout->addWidget(ProductionLineDataGroup);

  ProductionLineDataLayout = new QVBoxLayout();
  ProductionLineDataGroup->setLayout(ProductionLineDataLayout);

  ProductionLineDataView = new QTableView();
  ProductionLineDataLayout->addWidget(ProductionLineDataView);

  // Дополнительный холст
  ModelViewSublayout = new QHBoxLayout();
  ModelViewLayout->addLayout(ModelViewSublayout);

  // Данные бокса
  BoxDataGroup = new QGroupBox("Данные бокса, находящегося в процессе сборки");
  ModelViewSublayout->addWidget(BoxDataGroup);

  BoxDataLayout = new QVBoxLayout();
  BoxDataGroup->setLayout(BoxDataLayout);

  BoxDataView = new QTableView();
  BoxDataLayout->addWidget(BoxDataView);

  // Данные транспондера
  TransponderDataGroup =
      new QGroupBox("Данные последнего выпущенного транспондера");
  ModelViewSublayout->addWidget(TransponderDataGroup);

  TransponderDataLayout = new QVBoxLayout();
  TransponderDataGroup->setLayout(TransponderDataLayout);

  TransponderDataView = new QTableView();
  TransponderDataLayout->addWidget(TransponderDataView);
}
