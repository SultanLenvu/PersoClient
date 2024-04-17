#ifndef SHAREDQOBJECTCONTAINER_H
#define SHAREDQOBJECTCONTAINER_H

#include <QHash>
#include <QObject>
#include <QThread>

class SharedQObjectContainer : public QObject
{
  Q_OBJECT

 private:
  QHash<QString, std::shared_ptr<QObject>> Objects;

 public:
  explicit SharedQObjectContainer() = default;
  ~SharedQObjectContainer() = default;
  Q_DISABLE_COPY_MOVE(SharedQObjectContainer)

 public:
  void add(std::shared_ptr<QObject> obj);
  void remove(const QString& name);

  template <typename T>
  std::shared_ptr<T> get(const QString& name) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");

    if (!Objects.contains(name)) {
      return nullptr;
    }

    // Шерить объекты можно только между теми владельцами, которые находятся в
    // том же потоке QThread, что и разделяемый объект
    std::shared_ptr<QObject> ptr = Objects[name];
    if (ptr->thread() != QThread::currentThread()) {
      return nullptr;
    }

    return std::static_pointer_cast<T>(ptr);
  }
};

#endif // SHAREDQOBJECTCONTAINER_H
