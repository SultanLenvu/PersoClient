#include "log_system.h"

LogSystem::LogSystem(QObject *parent) : QObject(parent)
{
  
}

void LogSystem::programmerLog(const QString &log) {
  emit requestDisplayLog(QString("Programmer - ") + log);
}

void LogSystem::loadManagerLog(const QString &log) {
  emit requestDisplayLog(QString("Manager - ") + log);
}
