#include "log_system.h"

LogSystem::LogSystem(QObject *parent) : QObject(parent) { IsEnable = true; }

void LogSystem::programmerLog(const QString &log) {
  if (IsEnable)
    emit requestDisplayLog(QString("Programmer - ") + log);
}

void LogSystem::loadManagerLog(const QString &log) {
  if (IsEnable)
    emit requestDisplayLog(QString("Manager - ") + log);
}
