#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include "i_connection_manager.h"
#include "i_object_connector.h"

template <typename F,
          typename S,
          template <typename, typename>
          class ConnectionStrategy>

class ConnectionManager : public IConnectionManager {
  static_assert(std::is_base_of_v<QObject, F>,
                "F must be derived from QObject");
  static_assert(std::is_base_of_v<QObject, S>,
                "S must be derived from QObject");
  static_assert(
      std::is_base_of_v<IObjectConnector<F, S>, ConnectionStrategy<F, S>>,
      "ConnectionStrategy<F, S> must be derived from IObjectConnector<F, S>");

 private:
  const F* FObject;
  const S* SObject;
  ConnectionStrategy<F, S> Connector;

 public:
  explicit ConnectionManager(const F* fobj, const S* sobj)
      : FObject(fobj), SObject(sobj){};
  ~ConnectionManager() = default;
  Q_DISABLE_COPY_MOVE(ConnectionManager)

 public:  // IConnectionManager interface
  virtual void connect() override { Connector.plug(FObject, SObject); };
  virtual void disconnect() override { Connector.unplug(FObject, SObject); };
};

#endif // CONNECTIONMANAGER_H
