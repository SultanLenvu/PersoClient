#include "authorization_manager.h"

AuthorizationManager::AuthorizationManager(
    const QString& name,
    std::shared_ptr<IServerConnection> server)
    : NamedObject(name), LoggableObject(name), Server(server) {}

ReturnStatus AuthorizationManager::logOn(const StringDictionary& param,
                                         StringDictionary& productinoLineData) {
  ReturnStatus ret = ReturnStatus::NoError;

  ret = Server->connect();
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->launchProductionLine(param);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  ret = Server->getProductionLineData(productinoLineData);
  if (ret != ReturnStatus::NoError) {
    return ret;
  }

  return ReturnStatus::NoError;
}

ReturnStatus AuthorizationManager::logOut() {
  if (Server->isConnected()) {
    Server->shutdownProductionLine();
    Server->disconnect();
  }

  return ReturnStatus::NoError;
}
