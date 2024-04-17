#include "qobject_factory.h"

QObjectFactory::QObjectFactory(QThread* t) {
  assert(t);

  Thread = t;
  //  qDebug() << "Factory constructor running in thread:"
  //           << QThread::currentThread();

  moveToThread(Thread);
  //  qDebug() << "Factory binded to thread:" << thread();
}
