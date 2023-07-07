#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFile>
#include <QMutex>
#include <QObject>
#include <QString>

#include "../Environment/definitions.h"

class InterfaceProgrammer : public QObject
{
  Q_OBJECT
protected:
  QMutex Mutex;

public:
  explicit InterfaceProgrammer(QObject* parent);
  virtual ~InterfaceProgrammer();

public slots:
  virtual void connect(void) = 0;
  virtual void load(QFile *firmware) = 0;
  virtual void erase(void) = 0;

signals:
  void logging(const QString& log);
  void operationFinished(void);
};

#endif // ABSTRACTPROGRAMMER_H
