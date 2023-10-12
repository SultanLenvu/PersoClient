#include "log_system.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
  setObjectName("LogSystem");
  loadSettings();
}

LogSystem::~LogSystem() {}

void LogSystem::clear() {
  emit requestClearDisplayLog();
}

void LogSystem::generate(const QString& log) {
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

  SavePath = settings.value("log_system/save_path").toBool();
}
