#include "abstract_gui_subkernel.h"
#include "global_environment.h"
#include "widget_log_backend.h"

AbstractGuiSubkernel::AbstractGuiSubkernel() {
  connect(this, &AbstractGuiSubkernel::clearLogDisplay_signal,
          GlobalEnvironment::instance()->getObject<WidgetLogBackend>(
              "WidgetLogBackend"),
          &WidgetLogBackend::clear);
}
