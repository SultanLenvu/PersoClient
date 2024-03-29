#include "programmer_gui_subkernel.h"
#include "master_gui.h"
#include "programmer_manager.h"

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}

ProgrammerGuiSubkernel::~ProgrammerGuiSubkernel() {}

void ProgrammerGuiSubkernel::connectGui(AbstractGui* gui) {
  CurrentGui = gui;

  switch (CurrentGui->type()) {
    case AbstractGui::Authorization:
      connectAuthorizationGui();
      break;
    case AbstractGui::Master:
      connectMasterGui();
      break;
    case AbstractGui::ProductionAssembler:
      connectProductionAssemblerGui();
      break;
    case AbstractGui::ProductionTester:
      connectProductionTesterGui();
      break;
    default:
      break;
  }
}

void ProgrammerGuiSubkernel::connectManager(
    std::shared_ptr<AbstractManager> manager) {
  if (manager->type() != AbstractManager::Programmer) {
    return;
  }

  Manager = manager;
  connectProgrammerManager();
}

void ProgrammerGuiSubkernel::reset() {
  Manager.reset();
}

void ProgrammerGuiSubkernel::displayUcid_slot(
    const std::shared_ptr<QString> ucid) {}

void ProgrammerGuiSubkernel::connectAuthorizationGui() {}

void ProgrammerGuiSubkernel::connectMasterGui() {
  MasterGui* gui = dynamic_cast<MasterGui*>(CurrentGui);

  connect(gui->ProgramDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::programMemory_guiSlot);
  connect(gui->ReadDeviceFirmwareButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::readMemory_guiSlot);
  connect(gui->EraseDeviceButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::eraseMemory_guiSlot);

  connect(gui->ReadDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::readUserData_guiSlot);
  connect(gui->ProgramDeviceUserDataButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::programUserData_guiSlot);
  connect(gui->ReadDeviceUcidButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::readUcid_guiSlot);

  connect(gui->unlockMemoryButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::unlockMemory_guiSlot);
  connect(gui->lockMemoryButton, &QPushButton::clicked, this,
          &ProgrammerGuiSubkernel::lockMemory_guiSlot);
}

void ProgrammerGuiSubkernel::connectProductionAssemblerGui() {}

void ProgrammerGuiSubkernel::connectProductionTesterGui() {}

void ProgrammerGuiSubkernel::connectProgrammerManager() const {
  ProgrammerManager* manager = dynamic_cast<ProgrammerManager*>(Manager.get());

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
          &ProgrammerManager::readTransponderUcid);

  connect(this, &ProgrammerGuiSubkernel::unlockMemory_signal, manager,
          &ProgrammerManager::unlockMemory);
  connect(this, &ProgrammerGuiSubkernel::lockMemory_signal, manager,
          &ProgrammerManager::lockMemory);
}

void ProgrammerGuiSubkernel::programMemory_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<QString> path(new QString(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)")));
  emit programMemory_signal(path);
}

void ProgrammerGuiSubkernel::readMemory_guiSlot() {
  emit clearLogDisplay_signal();

  emit readMemory_signal();
}

void ProgrammerGuiSubkernel::eraseMemory_guiSlot() {
  emit clearLogDisplay_signal();

  emit eraseMemory_signal();
}

void ProgrammerGuiSubkernel::programUserData_guiSlot() {
  emit clearLogDisplay_signal();

  std::shared_ptr<QString> path(new QString(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", "", "Все файлы (*.*)")));
  emit programUserData_signal(path);
}

void ProgrammerGuiSubkernel::readUserData_guiSlot() {
  emit clearLogDisplay_signal();

  emit readUserData_signal();
}

void ProgrammerGuiSubkernel::readUcid_guiSlot() {
  emit readUcid_signal();

  emit clearLogDisplay_signal();
}

void ProgrammerGuiSubkernel::unlockMemory_guiSlot() {
  emit clearLogDisplay_signal();

  emit unlockMemory_signal();
}

void ProgrammerGuiSubkernel::lockMemory_guiSlot() {
  emit clearLogDisplay_signal();

  emit lockMemory_signal();
}
