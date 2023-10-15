#include "widget_log_backend.h"

WidgetLogBackend::WidgetLogBackend(QObject *parent)
  : LogBackend(parent) {}

void WidgetLogBackend::writeLogLine(const QString &str)
{
  emit displayLog_signal(str);
}

void WidgetLogBackend::clear()
{
  emit clearLogDisplay_signal();
}
