#include "asynchronous_object_space.h"

#include "async_production_manager.h"
#include "async_programmer.h"
#include "async_server_connection.h"
#include "async_sticker_printer.h"
#include "named_object_factory.h"

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

  Wrappers.emplace_back(
      factory.create<AsyncStickerPrinter>("AsyncStickerPrinter"));
  Wrappers.emplace_back(factory.create<AsyncProgrammer>("AsyncProgrammer"));
  Wrappers.emplace_back(
      factory.create<AsyncServerConnection>("AsyncServerConnection"));
  Wrappers.emplace_back(
      factory.create<AsyncProductionManager>("AsyncProductionManager"));
}
