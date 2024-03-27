#include "asynchronous_object_space.h"

#include "named_object_factory.h"
#include "production_async_wrapper.h"
#include "programmer_async_wrapper.h"
#include "sticker_printer_async_wrapper.h"

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
}