#ifndef SERVICEOBJECTSPACE_H
#define SERVICEOBJECTSPACE_H

#include <QThread>

#include "abstract_service_object_space.h"
#include "configuration_system.h"
#include "log_system.h"

class ServiceObjectSpace final : public AbstractServiceObjectSpace {
 private:
  QThread Thread;
  std::unique_ptr<LogSystem> Logger;
  std::unique_ptr<ConfigurationSystem> ConfigUpdater;

 public:
  explicit ServiceObjectSpace();
  ~ServiceObjectSpace();

 private:
  Q_DISABLE_COPY_MOVE(ServiceObjectSpace)
};

#endif  // SERVICEOBJECTSPACE_H
