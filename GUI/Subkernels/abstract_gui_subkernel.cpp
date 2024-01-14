#include "abstract_gui_subkernel.h"
#include "global_environment.h"
#include "log_system.h"

AbstractGuiSubkernel::AbstractGuiSubkernel(const QString& name)
    : QObject{nullptr} {
  setObjectName(name);

  connect(this, &AbstractGuiSubkernel::clearLogDisplay_signal,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::clear);
}

AbstractGuiSubkernel::~AbstractGuiSubkernel() {}

AbstractGuiSubkernel::AbstractGuiSubkernel() {}
