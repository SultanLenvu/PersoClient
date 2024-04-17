#include "qobject_container.h"

#include <QDebug>

void QObjectContainer::remove(const QString& name) {}

void QObjectContainer::add(QObject* obj) {
  Objects.insert(obj->objectName(), obj);

  Qt::ConnectionType conType = Qt::DirectConnection;
  if (obj->thread() != thread()) {
    conType = Qt::BlockingQueuedConnection;
  }
  connect(obj, &QObject::destroyed, this, &QObjectContainer::removeSlot,
          conType);
}

void QObjectContainer::removeSlot() {
  QString name = sender()->objectName();

  qDebug() << "Object: " << name;

  Objects.remove(name);
}
