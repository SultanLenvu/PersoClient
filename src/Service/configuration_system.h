#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "global_environment.h"
#include "named_object.h"

class ConfigurationSystem : public NamedObject {
  Q_OBJECT
 private:
  GlobalEnvironment* Global;

 public:
  Q_INVOKABLE explicit ConfigurationSystem(const QString& name);
  ~ConfigurationSystem() = default;

 private:
  Q_DISABLE_COPY_MOVE(ConfigurationSystem)

 private:
  void initApplication(void);
  void registerMetaTypes(void);

 signals:
  void applySettings_signal(void);
};

#endif  // CONFIGURATIONMANAGER_H
