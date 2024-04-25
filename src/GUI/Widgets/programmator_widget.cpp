#include "programmator_widget.h"
#include "global_environment.h"
#include "programmer_gui_subkernel.h"

ProgrammatorWidget::ProgrammatorWidget(QWidget *parent)
    : QWidget{parent}
{
  create();
  connectDependencies();
}

void ProgrammatorWidget::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  // Панель управления
  ControlPanel = new QGroupBox("Панель управления");
  MainLayout->addWidget(ControlPanel);

  ControlPanelLayout = new QVBoxLayout();
  ControlPanel->setLayout(ControlPanelLayout);

  ProgramDeviceButton = new QPushButton(QString("Ручная зарузка прошивки"));
  ControlPanelLayout->addWidget(ProgramDeviceButton);
  ReadDeviceFirmwareButton = new QPushButton(QString("Считать прошивку"));
  ControlPanelLayout->addWidget(ReadDeviceFirmwareButton);
  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  ControlPanelLayout->addWidget(EraseDeviceButton);
  ProgramDeviceUserDataButton =
      new QPushButton(QString("Загрузить  пользовательские данные"));
  ControlPanelLayout->addWidget(ProgramDeviceUserDataButton);
  ReadDeviceUserDataButton =
      new QPushButton(QString("Считать  пользовательские данные"));
  ControlPanelLayout->addWidget(ReadDeviceUserDataButton);
  ReadDeviceUcidButton = new QPushButton(QString("Считать  UCID"));
  ControlPanelLayout->addWidget(ReadDeviceUcidButton);

  ButtonVS1 =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ButtonVS1);

  UnlockMemoryButton = new QPushButton(QString("Разблокировать память"));
  ControlPanelLayout->addWidget(UnlockMemoryButton);
  LockMemoryButton = new QPushButton(QString("Заблокировать память"));
  ControlPanelLayout->addWidget(LockMemoryButton);
}

void ProgrammatorWidget::connectDependencies() {
  ProgrammerGuiSubkernel* pgs =
      GlobalEnvironment::instance()->getObject<ProgrammerGuiSubkernel>(
          "ProgrammerGuiSubkernel");

  connect(ProgramDeviceButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::programMemory);
  connect(ReadDeviceFirmwareButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::readMemory);
  connect(EraseDeviceButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::eraseMemory);

  connect(ReadDeviceUserDataButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::readUserData);
  connect(ProgramDeviceUserDataButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::programUserData);
  connect(ReadDeviceUcidButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::readUcid);

  connect(UnlockMemoryButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::unlockMemory);
  connect(LockMemoryButton, &QPushButton::clicked, pgs,
          &ProgrammerGuiSubkernel::lockMemory);
}
