#include "global_environment.h"

#include <QApplication>
#include <QThread>

GlobalEnvironment* GlobalEnvironment::instance() {
  static GlobalEnvironment context;

  return &context;
}

void GlobalEnvironment::registerObject(NamedObject* obj) {
  Objects.insert(obj->objectName(), obj);
}

void GlobalEnvironment::registerSharedObject(std::shared_ptr<NamedObject> obj) {
  SharedObjects.insert(obj->objectName(), obj);
}
