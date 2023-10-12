#include "log_system.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
  setObjectName("LogSystem");
  loadSettings();
}

LogSystem::~LogSystem() {}

void LogSystem::clear() {
  if (!GlobalEnableOption) {
    return;
  }

  emit requestClearDisplayLog();
}

void LogSystem::generate(const QString& log) {
  if (!GlobalEnableOption) {
    return;
  }

  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  emit requestDisplayLog(LogData);
}

void LogSystem::applySettings() {
  generate("LogSystem - Применение новых настроек. ");
  loadSettings();
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  QSettings settings;

  GlobalEnableOption = settings.value("log_system/global_enable").toBool();
}
