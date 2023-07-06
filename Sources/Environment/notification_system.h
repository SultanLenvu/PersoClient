#ifndef NOTIFICATION_SYSTEM_H
#define NOTIFICATION_SYSTEM_H

#include <QMessageBox>
#include <QObject>

#include "../Environment/load_manager.h"

class UserNotificationSystem : public QObject {
  Q_OBJECT

private:
  QWidget *Window;

public:
  explicit UserNotificationSystem(QObject *parent, QWidget *window);

public slots:
  void generateUserNotification(const QString &text);
};

#endif // NOTIFICATION_SYSTEM_H
