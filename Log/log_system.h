#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QHostAddress>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTime>
#include <QUdpSocket>

/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

 private:
  QString SavePath;

 public:
  LogSystem(QObject* parent);
  ~LogSystem();

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
