#include "widget_log_backend.h"

WidgetLogBackend::WidgetLogBackend(QObject* parent) : LogBackend(parent)
{
  setObjectName("WidgetLogBackend");
  loadSettings();
}

WidgetLogBackend::~WidgetLogBackend() {}

void WidgetLogBackend::writeLogLine(const QString& str)
{
  emit displayLog_signal(str);
}

void WidgetLogBackend::clear()
{
  emit clearLogDisplay_signal();
}

void WidgetLogBackend::applySettings()
{
  loadSettings();
}

void WidgetLogBackend::loadSettings() {}
