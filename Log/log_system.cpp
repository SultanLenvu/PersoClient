#include <QSettings>

#include "file_log_backend.h"
#include "global_environment.h"
#include "log_system.h"
#include "widget_log_backend.h"

LogSystem::LogSystem(const QString& name) : QObject(nullptr) {
  setObjectName(name);
  loadSettings();

  Backends.push_back(
      std::unique_ptr<LogBackend>(new WidgetLogBackend("WidgetLogBackend")));
  Backends.push_back(
      std::unique_ptr<LogBackend>(new FileLogBackend("FileLogBackend")));

  GlobalEnvironment::instance()->registerObject(this);
}

LogSystem::~LogSystem() {}

void LogSystem::clear() {
  for (auto it = Backends.begin(); it != Backends.end(); it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log) {
  if (!LogEnable) {
    return;
  }

  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (auto it = Backends.begin(); it != Backends.end(); it++) {
    (*it)->writeLogLine(LogData);
  }
}

void LogSystem::applySettings() {
  generate(objectName() + " - Применение новых настроек.");
  loadSettings();

  for (auto it = Backends.begin(); it != Backends.end(); it++) {
    (*it)->applySettings();
  }
}


/*
 * Приватные методы
 */

LogSystem::LogSystem() {}

void LogSystem::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
}
