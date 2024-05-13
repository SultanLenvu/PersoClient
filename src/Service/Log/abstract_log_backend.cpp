#include "abstract_log_backend.h"

#include <QDebug>
#include <QThread>

AbstractLogBackend::~AbstractLogBackend() {
  qDebug() << "AbstractLogBackend destructor: " << QThread::currentThread();
}
