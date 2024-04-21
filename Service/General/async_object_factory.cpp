#include "async_object_factory.h"

AsyncObjectFactory::AsyncObjectFactory(QThread* t) {
  assert(t);

  Thread = t;
  //  qDebug() << "Factory constructor running in thread:"
  //           << QThread::currentThread();

  moveToThread(Thread);
  //  qDebug() << "Factory binded to thread:" << thread();
}
