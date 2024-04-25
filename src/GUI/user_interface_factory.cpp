#include "user_interface_factory.h"
#include "assembler_unit_user_interface.h"
#include "authorization_user_interface.h"
#include "master_user_interface.h"
#include "server_commands_widget.h"
#include "tester_unit_user_interface.h"

#include "log_display_widget.h"
#include "production_manager_widget.h"
#include "programmator_widget.h"
#include "sticker_printer_widget.h"

QWidget* UserInterfaceFactory::createMasterUI(
    const ProductionManagerGuiSubkernel* pmgs,
    const ProgrammerGuiSubkernel* pgs,
    const StickerPrinterGuiSubkernel* spgs,
    const ServerConnectionGuiSubkernel* scgs) const {
  QWidget* mui = new MasterUserInterface(, new ProgrammatorWidget(),
                                         new StickerPrinterWidget(),
                                         new LogDisplayWidget());

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

QWidget* UserInterfaceFactory::createProductionManager(
    const ProductionManagerGuiSubkernel* pmgs) {
  QWidget* pmw = new ProductionManagerWidget();

  void (ProductionManagerGuiSubkernel::*mptr)(void) =
      &ProductionManagerGuiSubkernel::logOn;
  QObject::connect(LogOnPushButton, &QPushButton::clicked, augs, mptr);
  QObject::connect(LogOutPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::logOut);

  QObject::connect(RequestBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::requestBox);
  QObject::connect(RefundBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::refundCurrentBox);
  QObject::connect(CompleteCurrentBoxPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::completeCurrentBox);

  QObject::connect(ReleaseTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::releaseTransponder);
  QObject::connect(RereleaseTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::rereleaseTransponder);
  QObject::connect(RollbackTransponderPushButton, &QPushButton::clicked, augs,
                   &ProductionManagerGuiSubkernel::rollbackTransponder);

  ProductionLineDataView->setModel(&augs->productionLineModel());
  BoxDataView->setModel(&augs->boxModel());
  TransponderDataView->setModel(&augs->transponderModel());
}

QWidget* UserInterfaceFactory::createProgrammer(
    const ProgrammerGuiSubkernel* pmgs) {}

QWidget* UserInterfaceFactory::createServerCommands(
    const ServerConnectionGuiSubkernel* pmgs) {}

QWidget* UserInterfaceFactory::createStickerPrinter(
    const StickerPrinterGuiSubkernel* pmgs) {}

QWidget* UserInterfaceFactory::createLogDisplay() {}
