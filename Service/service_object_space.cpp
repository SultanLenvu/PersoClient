#include "service_object_space.h"

#include "global_environment.h"
#include "named_object_factory.h"

ServiceObjectSpace::ServiceObjectSpace() {
  Thread.start();

  NamedObjectFactory factory(&Thread);
  ConfigUpdater = std::unique_ptr<ConfigurationSystem>(
      factory.create<ConfigurationSystem>("ConfigurationSystem"));
  Logger = std::unique_ptr<LogSystem>(factory.create<LogSystem>("LogSystem"));
}

ServiceObjectSpace::~ServiceObjectSpace() {
  Thread.quit();
  Thread.wait();
}
