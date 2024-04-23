#include "user_interface_factory.h"
#include "assembler_unit_user_interface.h"
#include "authorization_user_interface.h"
#include "master_user_interface.h"
#include "server_commands_widget.h"
#include "tester_unit_user_interface.h"

QWidget* UserInterfaceFactory::createMasterUI(
    const ProductionManagerGuiSubkernel* pmgs,
    const ProgrammerGuiSubkernel* pgs,
    const StickerPrinterGuiSubkernel* spgs,
    const ServerConnectionGuiSubkernel* scgs) const {
  QWidget* mui = new MasterUserInterface();

  QWidget* scw = new ServerCommandsWidget();

  return mui;
}

QWidget* UserInterfaceFactory::createAssemblerUI(
    const ProductionManagerGuiSubkernel* pmgs) const {
  QWidget* auui = new AssemblerUnitUserInterface();

  return auui;
}

QWidget* UserInterfaceFactory::createTesterUI(
    const ProductionManagerGuiSubkernel* pmgs) const {
  QWidget* teei = new TesterUnitUserInterface();

  return teei;
}

QWidget* UserInterfaceFactory::createAuthorizationUI(
    const ProductionManagerGuiSubkernel* pmgs) const {
  QWidget* aui = new AuthorizationUserInterface();

  return aui;
}
