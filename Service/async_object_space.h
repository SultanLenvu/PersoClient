#ifndef ASYNCHRONOUSOBJECTSPACE_H
#define ASYNCHRONOUSOBJECTSPACE_H

#include <QObject>
#include <QThread>

#include "named_object_factory.h"

class AsyncObjectSpace final {
 private:
  QThread Thread;
  std::unique_ptr<NamedObjectFactory> Factory;
  std::vector<QObject*> Objects;

 public:
  explicit AsyncObjectSpace();
  ~AsyncObjectSpace();

 public:
  template <typename T>
  void add(const QString& name) {
    static_assert(std::is_base_of<NamedObject, T>::value,
                  "T must be derived from NamedObject");
    static_assert(
        std::is_constructible<T, const QString&>::value,
        "T must have a constructor with one argument of type const QString&");

    QObject* obj = Factory->create<T>(name);
    QObject::connect(&Thread, &QThread::finished, obj, &QObject::deleteLater);

    Objects.push_back(obj);
  };

  template <typename T>
  void add() {
    static_assert(std::is_base_of<QObject, T>::value,
                  "T must be derived from NamedObject");

    QObject* obj = Factory->create<T>();
    QObject::connect(&Thread, &QThread::finished, obj, &QObject::deleteLater);

    Objects.push_back(obj);
  };

 private:
  Q_DISABLE_COPY_MOVE(AsyncObjectSpace)
};

#endif // ASYNCHRONOUSOBJECTSPACE_H
