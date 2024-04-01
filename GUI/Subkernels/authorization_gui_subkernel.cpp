#include "authorization_gui_subkernel.h"

AuthorizationGuiSubkernel::AuthorizationGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel(name) {
  connectDependencies();
}

void AuthorizationGuiSubkernel::logOn() {}

void AuthorizationGuiSubkernel::logOnCompleted_slot() {
  //  if (Role == Assembler) {
  //    emit displayAssemblerUnitUserInterface();
  //  }

  //  if (Role == Tester) {
  //    emit displayProductionTesterGui();
  //  }
}

void AuthorizationGuiSubkernel::connectDependencies() {
  //  connect(manager, &ProductionManager::authorizationCompleted, this,
  //          &ProductionManagerGuiSubkernel::logOnCompleted_slot);
}
