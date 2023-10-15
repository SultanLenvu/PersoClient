#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QHostAddress>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTime>
#include <QUdpSocket>

#include <Log/log_backend.h>
#include <Log/text_stream_log_backend.h>
#include <Log/widget_log_backend.h>

/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

 private:
  QString SavePath;
  QList<LogBackend*> backends;
  WidgetLogBackend* WidgetLogger;
  TextStreamLogBackend* TextStreamLogger;

  static QMutex Mutex;

 public:
  ~LogSystem();
  WidgetLogBackend* getWidgetLogger();
  static LogSystem* instance(void);

 public slots:
  void clear(void);
  void generate(const QString& log);

  void applySettings(void);

 private:
  LogSystem(QObject* parent);
  Q_DISABLE_COPY(LogSystem)
  void loadSettings(void);
};

//==================================================================================

#endif  // LOGSYSTEM_H
