#include <QDateTime>
#include <QDir>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "log_system.h"

#include "Log/file_log_backend.h"
#include "Log/log_backend.h"
#include "Log/widget_log_backend.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
  setObjectName("LogSystem");
  loadSettings();

  WidgetLogger = new WidgetLogBackend(this);
  backends << WidgetLogger;

  TextStreamLogger = new FileLogBackend(this);
  backends << TextStreamLogger;
}

LogSystem::~LogSystem() {}

WidgetLogBackend* LogSystem::getWidgetLogger() {
  return WidgetLogger;
}

LogSystem* LogSystem::instance() {
  static LogSystem Logger(nullptr);
  return &Logger;
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

void LogSystem::applySettings() {
  generate("LogSystem - Применение новых настроек. ");
  loadSettings();
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {}
