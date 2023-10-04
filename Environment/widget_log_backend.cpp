#include <QObject>

#include "Environment/log_backend.h"
#include "Environment/widget_log_backend.h"
#include "GUI/master_gui.h"

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
