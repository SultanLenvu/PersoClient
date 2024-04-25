#include "global_environment.h"
#include "named_object.h"

NamedObject::NamedObject(const QString& name) : QObject{nullptr} {
  setObjectName(name);

  //  qDebug() << "NamedObject " << name << ": " << QThread::currentThread();

  GlobalEnvironment::instance()->registerObject(this);
}
