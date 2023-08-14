#include "log_system.h"

LogSystem::LogSystem(QObject *parent) : QObject(parent) { IsEnable = true; }

void LogSystem::setEnable(bool flag) {
  IsEnable = flag;
}

void LogSystem::clear() {
  if (!IsEnable) {
    return;
  }

  emit requestClearDisplayLog();
}

void LogSystem::generateLog(const QString& log) {
  if (!IsEnable) {
    return;
  }

  emit requestDisplayLog(QString("Manager - ") + log);
}
