#include <QSettings>
#include <QThread>

#include "widget_log_backend.h"

WidgetLogBackend::WidgetLogBackend(const QString& name)
    : NamedObject(name), LoggableObject(name) {
  doLoadSettings();
}

WidgetLogBackend::~WidgetLogBackend() {
  qDebug() << "WidgetLogBackend destructor: " << QThread::currentThread();
}

void WidgetLogBackend::writeMessage(const QString& str) {
  if (Enable) {
    emit displayLog_signal(str);
  }
}

void WidgetLogBackend::clear() {
  if (Enable) {
    emit clearLogDisplay_signal();
  }
}

void WidgetLogBackend::loadSettings() {
  sendLog("Загрузка настроек.");
  doLoadSettings();
}

void WidgetLogBackend::doLoadSettings() {
  QSettings settings;

  Enable = settings.value("log_system/display_enable").toBool();
}
