#include "asynchronous_object_space.h"

#include "async_production_manager.h"
#include "async_programmer.h"
#include "async_server_connection.h"
#include "async_sticker_printer.h"
#include "global_environment.h"
#include "jlink_exe_programmer.h"
#include "named_object_factory.h"
#include "perso_server_connection.h"
#include "te310_printer.h"

AsynchronousObjectSpace::AsynchronousObjectSpace() {
  Thread.start();
  createWrappers();
}

AsynchronousObjectSpace::~AsynchronousObjectSpace() {
  Thread.quit();
  Thread.wait();
}

void AsynchronousObjectSpace::createWrappers() {
  NamedObjectFactory factory(&Thread);

  std::shared_ptr<IStickerPrinter> sp(
      factory.create<TE310Printer>("TE310Printer"));
  GlobalEnvironment::instance()->registerSharedObject<IStickerPrinter>(
      "IStickerPrinter", sp);
  Wrappers.emplace_back(new AsyncStickerPrinter("AsyncStickerPrinter", sp));

  std::shared_ptr<IProgrammer> pr(
      factory.create<JLinkExeProgrammer>("JLinkExeProgrammer"));
  GlobalEnvironment::instance()->registerSharedObject<IProgrammer>(
      "IProgrammer", pr);
  Wrappers.emplace_back(new AsyncProgrammer("AsyncProgrammer", pr));

  std::shared_ptr<IServerConnection> sc(
      factory.create<PersoServerConnection>("PersoServerConnection"));
  GlobalEnvironment::instance()->registerSharedObject<IServerConnection>(
      "IServerConnection", sc);
  Wrappers.emplace_back(new AsyncServerConnection("AsyncServerConnection", sc));

  Wrappers.emplace_back(new AsyncProductionManager("AsyncProductionManager"));
}
