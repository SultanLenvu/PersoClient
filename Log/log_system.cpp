#include <QObject>
#include <QList>

#include "log_system.h"

#include "Log/log_backend.h"
#include "Log/widget_log_backend.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
  setObjectName("LogSystem");
  loadSettings();
  WidgetLogger = new WidgetLogBackend(this);
  backends << WidgetLogger;
}

LogSystem::~LogSystem() {
  delete WidgetLogger;
}

void LogSystem::clear() {
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log) {
  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++) {
    (*it)->writeLogLine(LogData);
  }
}

void LogSystem::addBackend(LogBackend *backend)
{
  backends << backend;
}

void LogSystem::removeBackend(LogBackend *backend)
{
  backends.removeOne(backend);
}

void LogSystem::applySettings() {
  generate("LogSystem - Применение новых настроек. ");
  loadSettings();
}

WidgetLogBackend *LogSystem::getWidgetLogger() {
  return WidgetLogger;
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  QSettings settings;

  SavePath = settings.value("log_system/save_path").toBool();
}
