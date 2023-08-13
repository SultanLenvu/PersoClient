#include "user_settings.h"

UserSettings::UserSettings(QObject* parent) : QObject(parent) {
  reset();
}

QString UserSettings::getPersoServerAddress() const {
  return PersoServerAddress.toString();
}

void UserSettings::setPersoServerAddress(const QString& addr) {
  PersoServerAddress = QHostAddress(addr);

  if (PersoServerAddress.isNull() == true)
    ValidationIndicator = false;
}

uint32_t UserSettings::getPersoServerPort() const {
  return PersoServerPort;
}

void UserSettings::setPersoServerPort(
    const QString newPersonalizationPersoServerPort) {
  PersoServerPort = newPersonalizationPersoServerPort.toInt();

  if ((PersoServerPort <= 0) || (PersoServerPort > IP_PORT_MAX_VALUE))
    ValidationIndicator = false;
}

bool UserSettings::isValid() const {
  return ValidationIndicator;
}

void UserSettings::reset() {
  PersoServerAddress = PERSO_SERVER_DEFAULT_IP;
  PersoServerPort = QString(PERSO_SERVER_DEFAULT_PORT).toInt();

  ValidationIndicator = true;
}
