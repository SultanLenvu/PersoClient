#include "log_system.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent)
{
  setObjectName("LogSystem");
  loadSettings();

  WidgetLogger = new WidgetLogBackend(this);
  Backends << WidgetLogger;

  FileLogger = new FileLogBackend(this);
  Backends << FileLogger;
}

LogSystem::~LogSystem() {}

WidgetLogBackend* LogSystem::getWidgetLogger()
{
  return WidgetLogger;
}

LogSystem* LogSystem::instance()
{
  static LogSystem Logger(nullptr);
  return &Logger;
}

void LogSystem::clear()
{
  for (QList<LogBackend*>::iterator it = Backends.begin(); it != Backends.end();
       it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log)
{
  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (QList<LogBackend*>::const_iterator it = Backends.constBegin();
       it != Backends.constEnd(); it++) {
    (*it)->writeLogLine(LogData);
  }
}

void LogSystem::applySettings()
{
  generate("LogSystem - Применение новых настроек. ");
  loadSettings();

  for (QList<LogBackend*>::const_iterator it = Backends.constBegin();
       it != Backends.constEnd(); it++) {
    (*it)->applySettings();
  }
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings()
{
  QSettings settings;
}
