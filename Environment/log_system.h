#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QObject>
#include <QTime>
#include <QList>

#include "Environment/log_backend.h"

/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

 private:
  QList<LogBackend*> backends;

 public:
  LogSystem(QObject* parent);
  ~LogSystem();

  void addBackend(LogBackend *backend);
  void removeBackend(LogBackend *backend);

  void clear(void);

 public slots:
  void generate(const QString& log);
};

//==================================================================================

#endif  // LOGSYSTEM_H
