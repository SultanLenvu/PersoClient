#ifndef NAMEDOBJECTFACTORY_H
#define NAMEDOBJECTFACTORY_H

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include <QThread>

class ThreadObjectFactory final : public QObject {
  Q_OBJECT

 private:
  QThread* Thread;

 public:
  explicit ThreadObjectFactory(QThread* t) {
    assert(t);
    Thread = t;
    moveToThread(Thread);
  }
  ~ThreadObjectFactory() = default;
  Q_DISABLE_COPY_MOVE(ThreadObjectFactory)

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
    };

    connect(this, &ThreadObjectFactory::createSignal, this, createFunc,
            conType);
    emit createSignal();
    disconnect(SIGNAL(createSignal()));
    return object;
  }

 signals:
  void createSignal(void);
};

#endif  // NAMEDOBJECTFACTORY_H
