#include "abstract_server_connection.h"
#include "global_environment.h"
#include "log_system.h"

AbstractServerConnection::AbstractServerConnection(const QString& name)
    : QObject{nullptr} {
  setObjectName(name);

  QObject::connect(this, &AbstractServerConnection::logging,
                   dynamic_cast<LogSystem*>(
                       GlobalEnvironment::instance()->getObject("LogSystem")),
                   &LogSystem::generate);
}

AbstractServerConnection::~AbstractServerConnection() {}

AbstractServerConnection::AbstractServerConnection() : QObject{nullptr} {}
