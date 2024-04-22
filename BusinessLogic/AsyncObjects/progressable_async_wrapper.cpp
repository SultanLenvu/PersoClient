#include "progressable_async_wrapper.h"

AbstractAsyncObject::AbstractAsyncObject(const QString& name)
    : LoggableObject(name) {}

void AbstractAsyncObject::initOperation(const QString& name) {
  sendLog(QString("Начало выполнения операции %1.").arg(name));
  emit executionStarted(name);
}

void AbstractAsyncObject::processOperationError(const QString& name,
                                                     ReturnStatus ret) {
  sendLog(QString("Получена ошибка при выполнении операции %1.").arg(name));
  emit executionFinished(name, ret);
}

void AbstractAsyncObject::completeOperation(const QString& name) {
  sendLog(QString("Операция %1 успешно выполнена.").arg(name));
  emit executionFinished(name, ReturnStatus::NoError);
}
