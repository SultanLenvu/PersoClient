#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <QObject>

class LogSystem : public QObject
{
  Q_OBJECT
public:
  explicit LogSystem(QObject *parent);

public slots:
  void programmerLog(const QString &log);
  void loadManagerLog(const QString &log);

signals:
  void requestDisplayLog(const QString &log);
};

#endif // LOG_SYSTEM_H
