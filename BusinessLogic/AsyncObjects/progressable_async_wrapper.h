#ifndef PROGRESSABLEASYNCWRAPPER_H
#define PROGRESSABLEASYNCWRAPPER_H

#include "abstract_gui_subkernel.h"
#include "i_async_wrapper.h"
#include "loggable_object.h"
#include "named_object.h"

class ProgressableAsyncWrapper : public NamedObject,
                                 public IAsyncWrapper,
                                 public LoggableObject {
  Q_OBJECT

 public:
  explicit ProgressableAsyncWrapper(const QString& name);
  virtual ~ProgressableAsyncWrapper() = default;
  Q_DISABLE_COPY_MOVE(ProgressableAsyncWrapper)

 public:
  template <typename T>
  void connectDependecies(AbstractGuiSubkernel subkernel) {
    T connector(subkernel);
    connector.connect();
  }

 protected:
  virtual void initOperation(const QString& name);
  virtual void processOperationError(const QString& name, ReturnStatus ret);
  virtual void completeOperation(const QString& name);

 signals:
  void executionStarted(const QString& name);
  void executionFinished(const QString& name, ReturnStatus ret);
};

#endif // PROGRESSABLEASYNCWRAPPER_H
