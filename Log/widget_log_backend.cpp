#include "widget_log_backend.h"

WidgetLogBackend::WidgetLogBackend(QObject *parent)
  : LogBackend(parent) {}

void WidgetLogBackend::writeLogLine(const QString &str)
{
  emit requestDisplayLog(str);
}

void WidgetLogBackend::clear()
{
  emit requestClearDisplayLog();
}
