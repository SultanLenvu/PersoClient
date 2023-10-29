#include "testing_gui.h"

TestingGUI::TestingGUI(QWidget* parent) : AbstractGUI(parent, Testing) {
  ControlPanelGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(ControlPanelGroup);

  ControlPanelLayout = new QVBoxLayout();
  ControlPanelGroup->setLayout(ControlPanelLayout);

  ReloadTransponderFirmwareButton =
      new QPushButton(QString("Перевыпустить транспондер"));
  ReloadTransponderFirmwareButton->setFont(QFont("Arial", 12, QFont::Bold));
  ControlPanelLayout->addWidget(ReloadTransponderFirmwareButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVerticalSpacer);

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

TestingGUI::~TestingGUI() {}

void TestingGUI::update() {}
