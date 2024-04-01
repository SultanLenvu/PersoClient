#include "global_environment.h"

#include <QApplication>
#include <QThread>

GlobalEnvironment* GlobalEnvironment::instance() {
  static GlobalEnvironment context;

  return &context;
}

void GlobalEnvironment::registerObject(QObject* obj) {
  QString name = obj->objectName();

  Objects.insert(name, obj);
}
