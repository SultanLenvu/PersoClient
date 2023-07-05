#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QObject>

class LogSystem : public QObject
{
  Q_OBJECT
public:
  explicit LogSystem(QObject *parent = nullptr);

signals:

};

#endif // LOGSYSTEM_H
