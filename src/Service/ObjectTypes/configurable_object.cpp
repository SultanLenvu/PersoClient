#include "configurable_object.h"

#include "configuration_system.h"
#include "global_environment.h"

ConfigurableObject::ConfigurableObject() {
  Connector =
      std::unique_ptr<SettingsApplyBundle>(new SettingsApplyBundle(this));
}

SettingsApplyBundle::SettingsApplyBundle(ConfigurableObject* object) {
  Object = object;

  ConfigurationSystem* cum =
      GlobalEnvironment::instance()->getObject<ConfigurationSystem>(
          "ConfigurationSystem");

  connect(cum, &ConfigurationSystem::applySettings_signal, this,
          &SettingsApplyBundle::apply);
}

void SettingsApplyBundle::apply() {
  Object->loadSettings();
}
