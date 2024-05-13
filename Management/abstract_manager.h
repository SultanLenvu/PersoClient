#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <types.h>
#include <QObject>

class AbstractManager : public QObject {
  Q_OBJECT
 public:
  enum Type {
    Production,
    Programmer,
    StickerPrinter,
  };
  Q_ENUM(Type)

 public:
  explicit AbstractManager(const QString& name);
  virtual ~AbstractManager();

  virtual void onInstanceThreadStarted(void) = 0;
  virtual Type type() const = 0;

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
