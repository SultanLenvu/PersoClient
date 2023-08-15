#include "production_gui.h"

ProductionGUI::ProductionGUI(QWidget* parent) : GUI(parent, Production) {}

void ProductionGUI::create(void) {
  // Группа основных кнопок
  MainButtonGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(MainButtonGroup);

  MainButtonLayout = new QVBoxLayout();
  MainButtonGroup->setLayout(MainButtonLayout);

  ProgramDeviceButton = new QPushButton(QString("Загрузить прошивку"));
  ProgramDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
  ProgramDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ProgramDeviceButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
  MainButtonLayout->addItem(ButtonVerticalSpacer);
}

void ProductionGUI::update() {}
