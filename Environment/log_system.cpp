#include "log_system.h"
#include "log_backend.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
}

LogSystem::~LogSystem() {}

void LogSystem::addBackend(LogBackend *backend)
{
  backends.append(backend);
}

void LogSystem::clear() {
  for (int i = 0; i < backends.size(); i++)
    backends[i]->clear();
}

void LogSystem::generate(const QString& log) {
  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (int i = 0; i < backends.size(); i++)
    backends[i]->writeLogLine(LogData);
}

/*
 * Приватные методы
 */
