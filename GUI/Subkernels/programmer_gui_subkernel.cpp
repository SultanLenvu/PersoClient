#include "programmer_gui_subkernel.h"

#include "master_gui.h"

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

ProgrammerGuiSubkernel::~ProgrammerGuiSubkernel() {}

void ProgrammerGuiSubkernel::setCurrentGui(std::shared_ptr<AbstractGui> gui) {
  CurrentGui = gui;

  switch (gui->type()) {
    case AbstractGui::Production:
      connectProductionGui();
      break;
    case AbstractGui::Testing:
      connectTestingGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    default:
      break;
  }
}

void ProgrammerGuiSubkernel::displayUcid_slot(
    const std::shared_ptr<QString> ucid) {}

void ProgrammerGuiSubkernel::connectProductionGui() {}

void ProgrammerGuiSubkernel::connectTestingGui() {}

void ProgrammerGuiSubkernel::connectMasterGui() {
  MasterGUI* gui = dynamic_cast<MasterGUI*>(CurrentGui.get());

  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::programMemory_guiSlot);
  connect(gui->ReadDeviceFirmwareButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::readMemory_guiSlot);
  connect(gui->EraseDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::eraseMemory_guiSlot);

  connect(gui->ReadDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::programUserData_guiSlot);
  connect(gui->ProgramDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::readUserData_guiSlot);

  connect(gui->UnlockDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::unlockMemory_guiSlot);
  connect(gui->LockDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::lockMemory_guiSlot);
}

void ProgrammerGuiSubkernel::programMemory_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<QString> path(new QString(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)")));
  emit programMemory_signal(path);
}

void ProgrammerGuiSubkernel::readMemory_guiSlot() {
  emit loggerClear_signal();

  emit readMemory_signal();
}

void ProgrammerGuiSubkernel::eraseMemory_guiSlot() {
  emit loggerClear_signal();

  emit eraseMemory_signal();
}

void ProgrammerGuiSubkernel::programUserData_guiSlot() {
  emit loggerClear_signal();

  std::shared_ptr<QString> path(new QString(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)")));
  emit programUserData_signal(path);
}

void ProgrammerGuiSubkernel::readUserData_guiSlot() {
  emit loggerClear_signal();

  emit readUserData_signal();
}

void ProgrammerGuiSubkernel::readUcid_guiSlot() {
  emit loggerClear_signal();
}

void ProgrammerGuiSubkernel::unlockMemory_guiSlot() {
  emit loggerClear_signal();

  emit unlockMemory_signal();
}

void ProgrammerGuiSubkernel::lockMemory_guiSlot() {
  emit loggerClear_signal();

  emit lockMemory_signal();
}
