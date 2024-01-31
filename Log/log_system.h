#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTime>
#include <QUdpSocket>

#include "log_backend.h"

/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

 private:
  bool LogEnable;
  int32_t MessageMaxSize;

  std::vector<std::unique_ptr<LogBackend>> Backends;

 public:
  explicit LogSystem(const QString& name);
  ~LogSystem();

 public slots:
  void clear(void);
  void generate(const QString& log);
  void applySettings(void);

 private:
  LogSystem();
  Q_DISABLE_COPY_MOVE(LogSystem)

  void loadSettings(void);
};

//==================================================================================

#endif  // LOGSYSTEM_H
