#include "widget_log_backend.h"
#include "global_environment.h"

WidgetLogBackend::WidgetLogBackend(const QString& name) : LogBackend(name) {
  loadSettings();

  GlobalEnvironment::instance()->registerObject(this);
}

WidgetLogBackend::~WidgetLogBackend() {}

void WidgetLogBackend::writeLogLine(const QString& str) {
  emit displayLog_signal(str);
}

void WidgetLogBackend::clear() {
  emit clearLogDisplay_signal();
}

void WidgetLogBackend::applySettings() {
  loadSettings();
}

void WidgetLogBackend::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/display_enable").toBool();
}
