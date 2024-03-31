#include "configurable_object.h"

#include "configuration_manager.h"
#include "global_environment.h"

ConfigurableObject::ConfigurableObject() {
  Connector =
      std::unique_ptr<SettingsApplyBundle>(new SettingsApplyBundle(this));
}

SettingsApplyBundle::SettingsApplyBundle(ConfigurableObject* object) {
  Object = object;

  ConfigurationManager* cum =
      GlobalEnvironment::instance()->getObject<ConfigurationManager>(
          "ConfigurationManager");

  connect(cum, &ConfigurationManager::applySettings_signal, this,
          &SettingsApplyBundle::apply);
}

SettingsApplyBundle::~SettingsApplyBundle() {}

void SettingsApplyBundle::apply() {
  Object->loadSettings();
}
