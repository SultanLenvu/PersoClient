#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <QHostAddress>
#include <QObject>

#include "definitions.h"

class UserSettings : public QObject {
  Q_OBJECT

 private:
  QHostAddress PersoServerAddress;
  uint32_t PersoServerPort;

  bool ValidationIndicator;

 public:
  explicit UserSettings(QObject* parent);

  QString getPersoServerAddress(void) const;
  void setPersoServerAddress(const QString& addr);

  uint32_t getPersoServerPort() const;
  void setPersoServerPort(const QString newPersonalizationPersoServerPort);

  bool isValid(void) const;

  void reset(void);
};

#endif  // USER_SETTINGS_H
