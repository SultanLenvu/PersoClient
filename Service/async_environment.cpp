#include "async_environment.h"

AsyncEnvironment::AsyncEnvironment() {
  Thread.start();
  Factory = std::make_unique<AsyncObjectFactory>(&Thread);
}

AsyncEnvironment::~AsyncEnvironment() {
  Thread.quit();
  Thread.wait();
}
