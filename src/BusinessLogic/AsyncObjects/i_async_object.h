#ifndef ABSTRACTASYNCWRAPPER_H
#define ABSTRACTASYNCWRAPPER_H

#include <QString>
#include "types.h"

class IAsyncObject {
 public:
  explicit IAsyncObject() = default;
  virtual ~IAsyncObject() = default;
  Q_DISABLE_COPY_MOVE(IAsyncObject)

 protected:
  virtual void initOperation(const QString& name) = 0;
  virtual void processOperationError(const QString& name, ReturnStatus ret) = 0;
  virtual void completeOperation(const QString& name) = 0;
};

#endif // ABSTRACTASYNCWRAPPER_H
