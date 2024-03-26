#include "abstract_gui_subkernel.h"
#include "global_environment.h"
#include "widget_log_backend.h"

AbstractGuiSubkernel::AbstractGuiSubkernel(const QString& name)
    : NamedObject{name} {
  connect(this, &AbstractGuiSubkernel::clearLogDisplay_signal,
          dynamic_cast<WidgetLogBackend*>(
              GlobalEnvironment::instance()->getObject("WidgetLogBackend")),
          &WidgetLogBackend::clear);
}
