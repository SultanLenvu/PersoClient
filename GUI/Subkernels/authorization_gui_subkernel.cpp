#include "authorization_gui_subkernel.h"

AuthorizationGuiSubkernel::AuthorizationGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel(name) {
  connectDependencies();
}

void AuthorizationGuiSubkernel::logOn_guiSlot() {}

void AuthorizationGuiSubkernel::logOnCompleted_slot() {
  if (Role == Assembler) {
    emit displayProductionAssemblerGui();
  }

  if (Role == Tester) {
    emit displayProductionTesterGui();
  }
}

void AuthorizationGuiSubkernel::connectDependencies() {
  connect(manager, &ProductionManager::authorizationCompleted, this,
          &ProductionGuiSubkernel::logOnCompleted_slot);
}
