#include "log_system.h"
#include "log_backend.h"

#include <QObject>
#include <QTime>
#include <QDateTime>
#include <QString>
#include <QList>

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
}

LogSystem::~LogSystem() {}

void LogSystem::addBackend(LogBackend *backend)
{
  backends << backend;
}

void LogSystem::removeBackend(LogBackend *backend)
{
  backends.removeOne(backend);
}

void LogSystem::clear() {
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++)
    (*it)->clear();
}

void LogSystem::generate(const QString& log) {
  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++)
    (*it)->writeLogLine(LogData);
}

/*
 * Приватные методы
 */
