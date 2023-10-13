#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QHostAddress>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTime>
#include <QUdpSocket>
#include <QList>

#include <Log/log_backend.h>
#include <Log/widget_log_backend.h>
#include <Log/text_stream_log_backend.h>


/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

  private:
    QString SavePath;
    QList<LogBackend*> backends;
    WidgetLogBackend *WidgetLogger; // meh

  public:
    LogSystem(QObject* parent);
    ~LogSystem();
    WidgetLogBackend *getWidgetLogger();
    void addBackend(LogBackend *backend);
    void removeBackend(LogBackend *backend);

  public slots:
    void clear(void);
    void generate(const QString& log);

    void applySettings(void);

  private:
    Q_DISABLE_COPY(LogSystem)
    void loadSettings(void);

  signals:
    void requestDisplayLog(const QString& logData);
    void requestClearDisplayLog(void);
};

//==================================================================================

#endif  // LOGSYSTEM_H
