#include "shared_qobject_container.h"

void SharedQObjectContainer::add(std::shared_ptr<QObject> obj) {
  Objects.insert(obj->objectName(), obj);
}

void SharedQObjectContainer::remove(const QString& name) {
  Objects.remove(name);
}
