#include "global_environment.h"

#include <QApplication>
#include <QThread>

GlobalEnvironment* GlobalEnvironment::instance() {
  static GlobalEnvironment context;

  //  qDebug() << "instance: " << QThread::currentThread();

  return &context;
}

void GlobalEnvironment::registerObject(QObject* obj) {
  Objects.insert(obj->objectName(), obj);

  Qt::ConnectionType conType = Qt::DirectConnection;
  if (obj->thread() != thread()) {
    conType = Qt::BlockingQueuedConnection;
  }
  connect(obj, &QObject::destroyed, this, &GlobalEnvironment::removeObject,
          conType);
}

void GlobalEnvironment::registerSharedObject(std::shared_ptr<QObject> obj) {
  SharedObjects.insert(obj->objectName(), obj);
}

void GlobalEnvironment::removeSharedObject(const QString& name) {
  SharedObjects.remove(name);
}

void GlobalEnvironment::removeObject() {
  QString name = sender()->objectName();

  qDebug() << "Object: " << name << ". Thread:" << QThread::currentThread();

  Objects.remove(name);
}
