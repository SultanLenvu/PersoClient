#include "log_system.h"

LogSystem::LogSystem(QObject *parent) : QObject(parent) { IsEnable = true; }

void LogSystem::generateLog(const QString& log) {
  if (IsEnable)
    emit requestDisplayLog(QString("Manager - ") + log);
}
