#ifndef NAMEDOBJECTFACTORY_H
#define NAMEDOBJECTFACTORY_H

#include <QMetaType>
#include <QObject>
#include <QThread>

#include "global_environment.h"
#include "named_object.h"

class NamedObjectFactory final : public QObject {
  Q_OBJECT

 private:
  QThread* Thread;
  const QMetaObject* CreatedMetaObject;

 public:
  NamedObjectFactory(QThread* thread);
  ~NamedObjectFactory() = default;

 public:
  template <typename T>
  typename std::enable_if<std::is_base_of<NamedObject, T>::value &&
                              std::is_constructible<T, const QString&>::value,
                          T*>::type
  create(const QString& name) {
    if (!Thread->isRunning()) {
      return nullptr;
    }

    CreatedMetaObject = QMetaType(qRegisterMetaType<T>()).metaObject();

    NamedObject* createdObject = nullptr;
    bool ok = true;
    Qt::ConnectionType conType = Qt::AutoConnection;
    if (QThread::currentThread() != Thread) {
      conType = Qt::BlockingQueuedConnection;
    } else {
      conType = Qt::DirectConnection;
    }

    ok = QMetaObject::invokeMethod(this, "doCreate", conType,
                                   qReturnArg(createdObject), name);

    if (!ok) {
      return nullptr;
    }

    return static_cast<T*>(createdObject);
  }

  template <typename T>
  std::shared_ptr<typename std::enable_if<
      std::is_base_of<NamedObject, T>::value &&
          std::is_constructible<T, const QString&>::value,
      T>::type>
  createShared(const QString& name) {
    std::shared_ptr<T> sharedObj =
        GlobalEnvironment::instance()->getSharedObject<T>(name);
    if (sharedObj) {
      return sharedObj;
    }

    sharedObj = std::shared_ptr<T>(create<T>(name));
    GlobalEnvironment::instance()->registerSharedObject(sharedObj);

    return sharedObj;
  }

 private:
  Q_DISABLE_COPY_MOVE(NamedObjectFactory)

 private slots:
  NamedObject* doCreate(const QString& objectName);

 signals:
};

#endif  // NAMEDOBJECTFACTORY_H
