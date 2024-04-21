#ifndef NAMEDOBJECTFACTORY_H
#define NAMEDOBJECTFACTORY_H

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include <QThread>

#include "global_environment.h"

class AsyncObjectFactory final : public QObject {
  Q_OBJECT

 private:
  QThread* Thread;

 public:
  explicit AsyncObjectFactory(QThread* thread);
  ~AsyncObjectFactory() = default;
  Q_DISABLE_COPY_MOVE(AsyncObjectFactory)

 public:
  template <typename T, typename... Args>
  T* create(Args... args) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");
    static_assert(
        std::is_constructible_v<T, Args...>,
        "T does not have a constructor that can be constructed from Args");

    if (!Thread->isRunning()) {
      return nullptr;
    }

    T* object = nullptr;

    // В зависимости от вызывающего потока
    Qt::ConnectionType conType = Qt::BlockingQueuedConnection;
    if (Thread == QThread::currentThread()) {
      conType = Qt::DirectConnection;
    }

    auto createFunc = [&object, &args...]() {
      object = new T(std::forward<Args>(args)...);
      //      qDebug() << "Lambda running in thread:" <<
      //      QThread::currentThread();
    };

    connect(this, &AsyncObjectFactory::createSignal, this, createFunc, conType);
    emit createSignal();
    disconnect(SIGNAL(createSignal()));
    return object;
  }

  template <typename T, typename... Args>
  std::shared_ptr<T> createShared(Args... args) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");
    static_assert(
        std::is_constructible_v<T, Args...>,
        "T does not have a constructor that can be constructed from Args");

    // Формально в конструкторе создаваемого QObject должно задаваться его имя
    std::shared_ptr<T> sharedObj =
        std::shared_ptr<T>(create<T>(std::forward<Args>(args)...));
    GlobalEnvironment::instance()->registerSharedObject(sharedObj);

    return sharedObj;
  }

 signals:
  void createSignal(void);
};

#endif  // NAMEDOBJECTFACTORY_H
