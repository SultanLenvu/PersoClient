#include <QObject>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QDir>

#include "log_system.h"

#include "Log/log_backend.h"
#include "Log/widget_log_backend.h"
#include "Log/text_stream_log_backend.h"

LogSystem::LogSystem(QObject* parent) : QObject(parent) {
  setObjectName("LogSystem");
  loadSettings();

  WidgetLogger = new WidgetLogBackend(this);
  backends << WidgetLogger;

  QDateTime datetime = QDateTime::currentDateTime();
  QStringList components;
  components << PROGRAM_NAME;
  components << "-";
  components << datetime.toString("yyyy-MM-dd_HH-mm-ss");
  components << ".log";
  QString baseName = components.join("");


  QDir logDirectory(SavePath);
  if (!logDirectory.exists()) {
    if (!logDirectory.mkpath("."))
      qWarning() << "Failed to create log directory: " << SavePath;
  }

  QString logFileName = logDirectory.filePath(baseName);

  TextStreamLogger = new TextStreamLogBackend(this, logFileName);
  if (TextStreamLogger->initialize())
    backends << TextStreamLogger;
  else
    qWarning() << "Failed to set up log file: " << logFileName;
}

LogSystem::~LogSystem() {
  delete WidgetLogger;
  delete TextStreamLogger;
}

void LogSystem::clear() {
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++) {
    (*it)->clear();
  }
}

void LogSystem::generate(const QString& log) {
  QTime time = QDateTime::currentDateTime().time();
  QString LogData = time.toString("hh:mm:ss.zzz - ") + log;
  for (QList<LogBackend*>::iterator it = backends.begin(); it != backends.end();
      it++) {
    (*it)->writeLogLine(LogData);
  }
}

void LogSystem::addBackend(LogBackend *backend)
{
  backends << backend;
}

void LogSystem::removeBackend(LogBackend *backend)
{
  backends.removeOne(backend);
}

void LogSystem::applySettings() {
  generate("LogSystem - Применение новых настроек. ");
  loadSettings();
}

WidgetLogBackend *LogSystem::getWidgetLogger() {
  return WidgetLogger;
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  QSettings settings;

  SavePath = settings.value("log_system/save_path").toBool();
}
