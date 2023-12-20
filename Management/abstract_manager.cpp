#include "abstract_manager.h"

AbstractManager::AbstractManager(const QString& name) : QObject{nullptr} {
  setObjectName(name);
}
