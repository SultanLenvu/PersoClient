#include "log_system.h"

#include <QNetworkDatagram>

#include "file_log_backend.h"
#include "widget_log_backend.h"

LogSystem::LogSystem(const QString& name)
    : NamedObject(name), LoggableObject(name) {
  doLoadSettings();

  Backends.emplace_back(new WidgetLogBackend("WidgetLogBackend"));
  Backends.emplace_back(new FileLogBackend("FileLogBackend"));
}

void LogSystem::generate(const QString& log,
                         const LoggableObject* source) const {
  QTime time = QDateTime::currentDateTime().time();

  QString LogMessage = QString("%1 - 0x%2 - %3 - %4")
                           .arg(time.toString("hh:mm:ss.zzz"),
                                QString::number(source->threadId(), 16),
                                source->sourceName(), log);

  for (auto it = Backends.begin(); it != Backends.end(); ++it) {
    (*it)->writeMessage(LogMessage);
  }
}

/*
 * Приватные методы
 */

void LogSystem::loadSettings() {
  sendLog("Загрузка настроек.");
  doLoadSettings();
}

void LogSystem::doLoadSettings() {
  QSettings settings;
}
