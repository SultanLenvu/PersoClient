#include "notification_system.h"

UserNotificationSystem::UserNotificationSystem(QObject *parent, QWidget *window)
    : QObject(parent) {
  Window = window;
}

void UserNotificationSystem::firmwareManagerNotification(
    FirmwareManager::PerfomingStatus status) {
  switch (status) {
  case FirmwareManager::Completed:
    QMessageBox::information(Window, "Менеджер", "Операция успешно выполнена",
                             QMessageBox::Ok);
    break;
  case FirmwareManager::Failed:
    QMessageBox::critical(Window, "Менеджер", "Операция провалилась",
                          QMessageBox::Ok);
    break;
  }
}
