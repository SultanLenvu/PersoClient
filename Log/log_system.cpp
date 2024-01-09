#include <QSettings>

#include "file_log_backend.h"
#include "log_system.h"
#include "widget_log_backend.h"

LogSystem::~LogSystem() {}

LogSystem::LogSystem(const QString& name) : QObject(nullptr) {
  setObjectName(name);
  loadSettings();

  Backends.push_back(
      std::unique_ptr<LogBackend>(new WidgetLogBackend("WidgetLogBackend")));

  Backends.push_back(
      std::unique_ptr<LogBackend>(new FileLogBackend("FileLogBackend")));
}

void LogSystem::clear() {
  for (std::vector<std::unique_ptr<LogBackend>>::iterator it = Backends.begin();
       it != Backends.end(); it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log) {
  if (!LogEnable) {
    return;
  }

  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (std::vector<std::unique_ptr<LogBackend>>::const_iterator it =
           Backends.begin();
       it != Backends.end(); it++) {
    (*it)->writeLogLine(LogData);
  }
}

void LogSystem::applySettings() {
  loadSettings();
}

LogSystem::LogSystem() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
}
