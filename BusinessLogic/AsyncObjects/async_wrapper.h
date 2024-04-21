#ifndef ASYNCWRAPPER_H
#define ASYNCWRAPPER_H

#include "named_object.h"
#include "types.h"

class AsyncWrapper : public QObject {
  Q_OBJECT

 private:
  NamedObject* Basis;

 public:
  explicit AsyncWrapper(const QString& name);
  ~AsyncWrapper() = default;

  template <typename... Args>
  bool asyncCall(const char* name, Args&&... args) {
    ReturnStatus ret = ReturnStatus::NoError;
    bool ok = QMetaObject::invokeMethod(Basis, name, Qt::QueuedConnection,
                                        qReturnArg(ret), args...);
    return ok;
  }

 private:
  Q_DISABLE_COPY_MOVE(AsyncWrapper)

 signals:
  void returnStatusReady(ReturnStatus ret);
  void executionStarted(const QString& name);
  void executionFinished(const QString& name);
};

#endif // ASYNCWRAPPER_H
