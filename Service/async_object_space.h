#ifndef ASYNCHRONOUSOBJECTSPACE_H
#define ASYNCHRONOUSOBJECTSPACE_H

#include <QObject>
#include <QThread>

#include "qobject_factory.h"

class AsyncObjectSpace final {
 private:
  QThread Thread;
  std::unique_ptr<QObjectFactory> Factory;
  std::vector<QObject*> Objects;

 public:
  explicit AsyncObjectSpace();
  ~AsyncObjectSpace();

 public:
  template <typename T, typename... Args>
  void add(Args... args) {
    static_assert(std::is_base_of<QObject, T>::value,
                  "T must be derived from QObject");
    static_assert(
        std::is_constructible<T, Args...>::value,
        "T does not have a constructor that can be constructed from Args");

    QObject* obj = Factory->create<T>(std::forward<Args>(args)...);
    QObject::connect(&Thread, &QThread::finished, obj, &QObject::deleteLater);

    Objects.push_back(obj);
  };

 private:
  Q_DISABLE_COPY_MOVE(AsyncObjectSpace)
};

#endif // ASYNCHRONOUSOBJECTSPACE_H
