#ifndef GLOBALENVIRONMENT_H
#define GLOBALENVIRONMENT_H

#include <QHash>
#include <QObject>
#include <QPointer>

class GlobalEnvironment {
 private:
  QHash<QString, QPointer<QObject>> Objects;
  QHash<QString, std::weak_ptr<void>> SharedObjects;

 public:
  ~GlobalEnvironment() = default;
  static GlobalEnvironment* instance(void);

 public:
  void registerObject(QObject* obj);
  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<QObject, T>>>
  T* getObject(const QString& name) {
    if (!Objects.contains(name)) {
      return nullptr;
    }

    if (!Objects.value(name)) {
      Objects.remove(name);
      return nullptr;
    }

    return static_cast<T*>(Objects.value(name).get());
  }

 public:
  template <typename T>
  void registerSharedObject(const QString& name, std::shared_ptr<T> obj) {
    SharedObjects.insert(name, obj);
  }

  template <typename T>
  std::shared_ptr<T> getSharedObject(const QString& name) {
    return std::static_pointer_cast<T>(SharedObjects[name].lock());
  }

 private:
  explicit GlobalEnvironment() = default;
  Q_DISABLE_COPY_MOVE(GlobalEnvironment)

 signals:
};

#endif  // GLOBALENVIRONMENT_H
