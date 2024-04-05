#ifndef GLOBALENVIRONMENT_H
#define GLOBALENVIRONMENT_H

#include <QHash>
#include <QPointer>

#include "named_object.h"

class GlobalEnvironment {
 private:
  QHash<QString, QPointer<NamedObject>> Objects;
  QHash<QString, std::weak_ptr<NamedObject>> SharedObjects;

 public:
  ~GlobalEnvironment() = default;
  static GlobalEnvironment* instance(void);

 public:
  void registerObject(NamedObject* obj);

  template <typename T,
            typename = std::enable_if_t<std::is_base_of_v<NamedObject, T>>>
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
  void registerSharedObject(std::shared_ptr<NamedObject> obj);

  template <typename T>
  std::shared_ptr<T> getSharedObject(const QString& name) {
    if (!SharedObjects.contains(name)) {
      return nullptr;
    }

    return std::static_pointer_cast<T>(SharedObjects[name].lock());
  }

 private:
  explicit GlobalEnvironment() = default;
  Q_DISABLE_COPY_MOVE(GlobalEnvironment)

 signals:
};

#endif  // GLOBALENVIRONMENT_H
