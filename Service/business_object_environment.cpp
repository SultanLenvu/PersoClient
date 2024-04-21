#include "business_object_environment.h"

#include "jlink_exe_programmer.h"
#include "perso_server_connection.h"
#include "production_manager.h"
#include "te310_printer.h"

#include "production_connector.h"
#include "production_context_connector.h"
#include "programmer_connector.h"
#include "server_connection_connector.h"
#include "sticker_printer_connector.h"

BusinessObjectEnvironment::BusinessObjectEnvironment(
    const ProductionManagerGuiSubkernel* pmgs,
    const ProgrammerGuiSubkernel* pgs,
    const StickerPrinterGuiSubkernel* spgs,
    const ServerConnectionGuiSubkernel* scgs) {
  createObjects();

  ProductionConnector<ProductionManagerGuiSubkernel, AsyncProductionManager> c1;
  c1.plug(pmgs, Manager.get());

  ProductionContextConnector<ProductionUnitContext,
                             ProductionManagerGuiSubkernel>
      c2;
  c2.plug(Context.get(), pmgs);

  StickerPrinterConnector<StickerPrinterGuiSubkernel, AsyncStickerPrinter> c3;
  c3.plug(spgs, StickerPrinter.get());

  ProgrammerConnector<ProgrammerGuiSubkernel, AsyncProgrammer> c4;
  c4.plug(pgs, Programmer.get());

  ServerConnectionConnector<ServerConnectionGuiSubkernel, AsyncServerConnection>
      c5;
  c5.plug(scgs, ServerConnection.get());
}

void BusinessObjectEnvironment::createObjects() {
  std::shared_ptr<TE310Printer> sp =
      std::make_shared<TE310Printer>("TE310Printer");
  StickerPrinter =
      std::make_unique<AsyncStickerPrinter>("AsyncStickerPrinter", sp);

  std::shared_ptr<JLinkExeProgrammer> p =
      std::make_shared<JLinkExeProgrammer>("JLinkExeProgrammer");
  Programmer = std::make_unique<AsyncProgrammer>("AsyncProgrammer", p);

  std::shared_ptr<ProductionUnitContext> Context =
      std::make_shared<ProductionUnitContext>("ProductionUnitContext");
  std::shared_ptr<PersoServerConnection> sc =
      std::make_shared<PersoServerConnection>("PersoServerConnection");
  ServerConnection = std::make_unique<AsyncServerConnection>(
      "AsyncServerConnection", Context, sc);

  std::unique_ptr<ProductionManager> manager =
      std::make_unique<ProductionManager>("ProductionManager", Context, sc, sp,
                                          p);
  Manager = std::make_unique<AsyncProductionManager>("AsyncProductionManager",
                                                     std::move(manager));
}
