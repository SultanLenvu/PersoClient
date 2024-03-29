#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H

#include "abstract_manager.h"
#include "i_server_connection.h"
#include "loggable_object.h"
#include "named_object.h"

class AuthorizationManager : public NamedObject,
                             public AbstractManager,
                             public LoggableObject {
  Q_OBJECT

 private:
  std::shared_ptr<IServerConnection> Server;

 public:
  explicit AuthorizationManager(const QString& name,
                                std::shared_ptr<IServerConnection> server);
  ~AuthorizationManager() = default;

 public slots:
  ReturnStatus logOn(const StringDictionary& param,
                     StringDictionary& productinoLineData);
  ReturnStatus logOut(void);
};

#endif  // AUTHORIZATIONMANAGER_H
