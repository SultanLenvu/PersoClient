#ifndef ASYNCENVIRONMENT_H
#define ASYNCENVIRONMENT_H

#include <QObject>
#include <QThread>

#include "async_object_factory.h"

class AsyncEnvironment final : public QObject {
  Q_OBJECT

 private:
  QThread Thread;
  std::unique_ptr<AsyncObjectFactory> Factory;
  std::vector<std::unique_ptr<QObject>> Objects;

 public:
  explicit AsyncEnvironment();
  ~AsyncEnvironment();
  Q_DISABLE_COPY_MOVE(AsyncEnvironment)

 public:
  template <typename T, typename... Args>
  void add(Args... args) {
    static_assert(std::is_base_of<QObject, T>::value,
                  "T must be derived from QObject");
    static_assert(
        std::is_constructible<T, Args...>::value,
        "T does not have a constructor that can be constructed from Args");

    QObject* obj = Factory->create<T>(std::forward<Args>(args)...);
    connect(&Thread, &QThread::finished, obj, &QObject::deleteLater);

    Objects.emplace_back(obj);
  };
};

#endif  // ASYNCENVIRONMENT_H
