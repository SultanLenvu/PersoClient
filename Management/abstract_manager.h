#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <types.h>
#include <QObject>

class AbstractManager : public QObject {
  Q_OBJECT
 public:
  explicit AbstractManager(const QString& name);
  virtual ~AbstractManager();

 public slots:
  virtual void applySettings(void) = 0;

 private:
  AbstractManager();
  Q_DISABLE_COPY_MOVE(AbstractManager);

 signals:
  void logging(const QString& log);
  void executionStarted(const QString& opName);
  void executionFinished(const QString& opName, ReturnStatus ret);
};

#endif  // ABSTRACTMANAGER_H
