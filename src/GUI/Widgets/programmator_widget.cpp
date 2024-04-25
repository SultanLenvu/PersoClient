#include "programmator_widget.h"

#include <QFileDialog>

ProgrammatorWidget::ProgrammatorWidget(QWidget *parent)
    : QWidget{parent}
{
  create();
  connectInternals();
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

void ProgrammatorWidget::connectInternals() {
  connect(ProgramDeviceButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::programMemory);
  connect(ReadDeviceFirmwareButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::readMemory_trigger);
  connect(EraseDeviceButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::eraseMemory_trigger);

  connect(ReadDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::readUserData_trigger);
  connect(ProgramDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::programUserData);
  connect(ReadDeviceUcidButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::readUcid_trigger);

  connect(UnlockMemoryButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::unlockMemory_trigger);
  connect(LockMemoryButton, &QPushButton::clicked, this,
          &ProgrammatorWidget::lockMemory_trigger);
}

void ProgrammatorWidget::programMemory() {
  QString path(QFileDialog::getOpenFileName(nullptr, "Выберите файл", "",
                                            "Все файлы (*.*)"));

  if (path.isEmpty()) {
    return;
  }

  emit programMemory_trigger(path);
}

void ProgrammatorWidget::programUserData() {
  QString path(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", QDir::currentPath(), "Все файлы (*.*)"));

  if (path.isEmpty()) {
    return;
  }

  emit programUserData_trigger(path);
}
