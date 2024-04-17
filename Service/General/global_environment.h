#ifndef GLOBALENVIRONMENT_H
#define GLOBALENVIRONMENT_H

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QThread>

class GlobalEnvironment : public QObject {
  Q_OBJECT

 private:
  QHash<QString, QObject*> Objects;
  QHash<QString, std::shared_ptr<QObject>> SharedObjects;

 public:
  ~GlobalEnvironment() = default;
  static GlobalEnvironment* instance(void);

 public:
  void registerObject(QObject* obj);

  template <typename T>
  T* getObject(const QString& name) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");

    if (!Objects.contains(name)) {
      return nullptr;
    }

    if (!Objects.value(name)) {
      Objects.remove(name);
      return nullptr;
    }

    return static_cast<T*>(Objects.value(name));
  }

 public:
  void registerSharedObject(std::shared_ptr<QObject> obj);
  void removeSharedObject(const QString& name);

  template <typename T>
  std::shared_ptr<T> getSharedObject(const QString& name) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");

    if (!SharedObjects.contains(name)) {
      return nullptr;
    }

    std::shared_ptr<QObject> ptr = SharedObjects[name];
    if (!ptr) {
      SharedObjects.remove(name);
      return nullptr;
    }

    // Шерить объекты можно только между теми владельцами, которые находятся в
    // том же потоке QThread, что и разделяемый объект
    if (ptr->thread() != QThread::currentThread()) {
      return nullptr;
    }

    return std::static_pointer_cast<T>(ptr);
  }

 private:
  explicit GlobalEnvironment() = default;
  Q_DISABLE_COPY_MOVE(GlobalEnvironment)

 private slots:
  void removeObject(void);

 signals:
};

#endif  // GLOBALENVIRONMENT_H
