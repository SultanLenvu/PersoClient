#include "notification_system.h"

UserNotificationSystem::UserNotificationSystem(QObject *parent, QWidget *window)
    : QObject(parent) {
  Window = window;
}

void UserNotificationSystem::generateUserNotification(const QString &text) {
  QMessageBox::information(Window, "Manager", text, QMessageBox::Ok);
}
