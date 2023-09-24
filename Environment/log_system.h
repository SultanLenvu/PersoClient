#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QObject>
#include <QTime>

/* Глобальная система логгирования */
//==================================================================================

class LogSystem : public QObject {
  Q_OBJECT

 private:

 public:
  LogSystem(QObject* parent);
  ~LogSystem();

 public:
  void clear(void);

 public slots:
  void generate(const QString& log);

 signals:
  void requestDisplayLog(const QString& logData);
  void requestClearDisplayLog(void);
};

//==================================================================================

#endif  // LOGSYSTEM_H
