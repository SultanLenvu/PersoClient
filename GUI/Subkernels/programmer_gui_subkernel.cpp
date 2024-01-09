#include "programmer_gui_subkernel.h"

#include "master_gui.h"

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

ProgrammerGuiSubkernel::~ProgrammerGuiSubkernel() {}

void ProgrammerGuiSubkernel::connectAuthorizationGui(
    std::shared_ptr<AuthorizationGui> gui) {}

void ProgrammerGuiSubkernel::connectMasterGui(std::shared_ptr<MasterGui> gui) {
  CurrentGui = gui;

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

  connect(gui->unlockMemoryButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::unlockMemory_guiSlot);
  connect(gui->lockMemoryButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::lockMemory_guiSlot);
}

void ProgrammerGuiSubkernel::connectProductionAssemblerGui(
    std::shared_ptr<ProductionAssemblerGui> gui) {
  CurrentGui = gui;
}

void ProgrammerGuiSubkernel::connectProductionTesterGui(
    std::shared_ptr<ProductionTesterGui> gui) {
  CurrentGui = gui;
}

void ProgrammerGuiSubkernel::resetCurrentGui() {
  CurrentGui.reset();
}

void ProgrammerGuiSubkernel::connectManager(
    const ProgrammerManager* manager) const {
  connect(this, &ProgrammerGuiSubkernel::programMemory_signal, manager,
          &ProgrammerManager::programMemory);
  connect(this, &ProgrammerGuiSubkernel::readMemory_signal, manager,
          &ProgrammerManager::readMemory);
  connect(this, &ProgrammerGuiSubkernel::eraseMemory_signal, manager,
          &ProgrammerManager::eraseMemory);

  connect(this, &ProgrammerGuiSubkernel::programUserData_signal, manager,
          &ProgrammerManager::programUserData);
  connect(this, &ProgrammerGuiSubkernel::readUserData_signal, manager,
          &ProgrammerManager::readUserData);
  connect(this, &ProgrammerGuiSubkernel::readUcid_signal, manager,
          &ProgrammerManager::readUcid);

  connect(this, &ProgrammerGuiSubkernel::unlockMemory_signal, manager,
          &ProgrammerManager::unlockMemory);
  connect(this, &ProgrammerGuiSubkernel::lockMemory_signal, manager,
          &ProgrammerManager::lockMemory);
}

void ProgrammerGuiSubkernel::displayUcid_slot(
    const std::shared_ptr<QString> ucid) {}

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
