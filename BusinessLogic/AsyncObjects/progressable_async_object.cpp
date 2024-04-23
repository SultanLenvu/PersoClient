#include "progressable_async_object.h"

void AbstractAsyncObject::initOperation(const QString& name) {
  emit executionStarted(name);
}

void AbstractAsyncObject::processOperationError(const QString& name,
                                                     ReturnStatus ret) {
  emit executionFinished(name, ret);
}

void AbstractAsyncObject::completeOperation(const QString& name) {
  emit executionFinished(name, ReturnStatus::NoError);
}
