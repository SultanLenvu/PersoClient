#include "production_assembler_gui.h"
#include "global_environment.h"
#include "production_gui_subkernel.h"

ProductionAssemblerGui::ProductionAssemblerGui(QWidget* parent)
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

void ProductionAssemblerGui::connectDependecies() {
  ProductionGuiSubkernel* pgs =
      GlobalEnvironment::instance()->getObject<ProductionGuiSubkernel>(
          "ProductionGuiSubkernel");

  connect(RequestBoxButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::requestBox_guiSlot);
  connect(RefundCurrentBoxButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::refundCurrentBox_guiSlot);
  connect(CompleteCurrentBoxButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::completeCurrentBox_guiSlot);

  connect(ReleaseTransponderButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::releaseTransponder_guiSlot);
  connect(RereleaseTransponderButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::rereleaseTransponder_guiSlot);
  connect(RollbackTransponderPushButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::rollbackTransponder_guiSlot);

  connect(PrintBoxStickerButton, &QPushButton::clicked, pgs,
          &ProductionGuiSubkernel::printBoxSticker_guiSlot);

  // Связывание моделей и представлений
  ProductionLineDataView->setModel(&pgs->ProductionLineModel);
  TransponderDataView->setModel(&pgs->TransponderDataModel);
  BoxDataView->setModel(&pgs->BoxDataModel);
}

void ProductionAssemblerGui::createBoxGroup() {
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

void ProductionAssemblerGui::createTransponderGroup() {
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

void ProductionAssemblerGui::createPrinterStickerGroup() {
  PrinterStickerGroup = new QGroupBox("Принтер стикеров");
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
}

void ProductionAssemblerGui::createModelViews() {
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
