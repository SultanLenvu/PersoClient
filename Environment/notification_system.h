#ifndef NOTIFICATION_SYSTEM_H
#define NOTIFICATION_SYSTEM_H

#include <QMessageBox>
#include <QObject>

#include "../Environment/firmware_manager.h"

class UserNotificationSystem : public QObject {
  Q_OBJECT

private:
  QWidget *Window;

public:
  explicit UserNotificationSystem(QObject *parent, QWidget *window);

public slots:
  void firmwareManagerNotification(FirmwareManager::PerfomingStatus status);
};

#endif // NOTIFICATION_SYSTEM_H
