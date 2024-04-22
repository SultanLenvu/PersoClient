#ifndef PROGRESSABLEASYNCWRAPPER_H
#define PROGRESSABLEASYNCWRAPPER_H

#include "i_async_wrapper.h"
#include "loggable_object.h"

class AbstractAsyncObject : public QObject,
                            public IAsyncWrapper,
                            public LoggableObject {
  Q_OBJECT

 public:
  explicit AbstractAsyncObject(const QString& name);
  virtual ~AbstractAsyncObject() = default;
  Q_DISABLE_COPY_MOVE(AbstractAsyncObject)

 protected:
  virtual void initOperation(const QString& name);
  virtual void processOperationError(const QString& name, ReturnStatus ret);
  virtual void completeOperation(const QString& name);

 signals:
  void executionStarted(const QString& name);
  void executionFinished(const QString& name, ReturnStatus ret);
};

#endif // PROGRESSABLEASYNCWRAPPER_H
