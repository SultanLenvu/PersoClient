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

#include "abstract_log_backend.h"
#include "configurable_object.h"
#include "loggable_object.h"
#include "named_object.h"

/* Глобальная система логгирования */
//==================================================================================

class LogSystem final : public NamedObject,
                        public ConfigurableObject,
                        public LoggableObject {
  Q_OBJECT

 private:
  std::vector<std::unique_ptr<AbstractLogBackend>> Backends;

 public:
  Q_INVOKABLE LogSystem(const QString& name);
  ~LogSystem() = default;

 public slots:
  void generate(const QString& log, const LoggableObject* source) const;

 private:
  Q_DISABLE_COPY_MOVE(LogSystem)
  virtual void loadSettings(void) override;
  void doLoadSettings(void);
};

//==================================================================================

#endif  // LOGSYSTEM_H
