#include "abstract_server_connection.h"

AbstractServerConnection::AbstractServerConnection(const QString& name)
    : QObject{nullptr} {
  setObjectName(name);
}

AbstractServerConnection::~AbstractServerConnection() {}

AbstractServerConnection::AbstractServerConnection() : QObject{nullptr} {}
