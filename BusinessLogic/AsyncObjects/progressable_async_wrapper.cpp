#include "progressable_async_wrapper.h"

ProgressableAsyncWrapper::ProgressableAsyncWrapper(const QString& name)
    : NamedObject(name), LoggableObject(name) {
}

void ProgressableAsyncWrapper::initOperation(const QString& name) {
  sendLog(QString("Начало выполнения операции %1.").arg(name));
  emit executionStarted(name);
}

void ProgressableAsyncWrapper::processOperationError(const QString& name,
                                                     ReturnStatus ret) {
  sendLog(QString("Получена ошибка при выполнении операции %1.").arg(name));
  emit executionFinished(name, ret);
}

void ProgressableAsyncWrapper::completeOperation(const QString& name) {
  sendLog(QString("Операция %1 успешно выполнена.").arg(name));
  emit executionFinished(name, ReturnStatus::NoError);
}
