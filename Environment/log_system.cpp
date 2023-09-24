#include "log_system.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
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

/*
 * Приватные методы
 */
