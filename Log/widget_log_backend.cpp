#include "widget_log_backend.h"

WidgetLogBackend::WidgetLogBackend(QObject *parent, MasterGUI *gui)
  : LogBackend(parent)
{
  connect(this, &WidgetLogBackend::requestDisplayLog,
      gui, &MasterGUI::displayLogData);
  connect(this, &WidgetLogBackend::requestClearDisplayLog,
      gui, &MasterGUI::clearLogDataDisplay);
}

void WidgetLogBackend::writeLogLine(const QString &str)
{
  emit requestDisplayLog(str);
}

void WidgetLogBackend::clear()
{
  emit requestClearDisplayLog();
}
