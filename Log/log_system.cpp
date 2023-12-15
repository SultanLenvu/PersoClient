#include <QMutexLocker>

#include "log_system.h"

LogSystem::~LogSystem() {}

LogSystem* LogSystem::instance() {
  static LogSystem Logger("LogSystem");
  return &Logger;
}

void LogSystem::clear() {
  QMutexLocker lock(&mutex);

  for (QList<LogBackend*>::const_iterator it = Backends.begin();
       it != Backends.end(); it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log) {
  if (!LogEnable) {
    return;
  }

  QMutexLocker lock(&mutex);

  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (QList<LogBackend*>::const_iterator it = Backends.begin();
       it != Backends.end(); it++) {
    (*it)->writeLogLine(LogData);
  }
}

LogSystem::LogSystem() {}

LogSystem::LogSystem(const QString& name) : QObject(nullptr) {
  setObjectName(name);
  loadSettings();

  WidgetLogger = new WidgetLogBackend(this);
  Backends << WidgetLogger;

  FileLogger = new FileLogBackend(this);
  Backends << FileLogger;
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
}
