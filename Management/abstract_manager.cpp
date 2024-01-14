#include "abstract_manager.h"
#include "global_environment.h"
#include "interaction_system.h"
#include "log_system.h"

AbstractManager::AbstractManager(const QString& name) : QObject{nullptr} {
  setObjectName(name);

  GlobalEnvironment::instance()->registerObject(this);

  connect(this, &AbstractManager::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
  connect(this, &AbstractManager::executionStarted,
          dynamic_cast<InteractionSystem*>(
              GlobalEnvironment::instance()->getObject("InteractionSystem")),
          &InteractionSystem::processOperationStart);
  connect(this, &AbstractManager::executionFinished,
          dynamic_cast<InteractionSystem*>(
              GlobalEnvironment::instance()->getObject("InteractionSystem")),
          &InteractionSystem::processOperationFinish);
}

AbstractManager::~AbstractManager() {}
