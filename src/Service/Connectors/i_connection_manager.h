#ifndef ICONNECTIONMANAGER_H
#define ICONNECTIONMANAGER_H

#include <QObject>
#include <QtClassHelperMacros>

class IConnectionManager
{
 public:
  explicit IConnectionManager() = default;
  ~IConnectionManager() = default;
  Q_DISABLE_COPY_MOVE(IConnectionManager)

 public:
  virtual void connect() = 0;
  virtual void disconnect() = 0;
};

#endif // ICONNECTIONMANAGER_H
