#include "user_Interaction_system.h"

UserInteractionSystem::UserInteractionSystem(QObject *parent, QWidget *window)
    : QObject(parent) {
  ParentWindow = window;
}

void UserInteractionSystem::firmwareManagerNotification(
    FirmwareManager::PerfomingStatus status) {
  switch (status) {
  case FirmwareManager::Completed:
    QMessageBox::information(ParentWindow, "Менеджер",
                             "Операция успешно выполнена", QMessageBox::Ok);
    break;
  case FirmwareManager::Failed:
    QMessageBox::critical(ParentWindow, "Менеджер", "Операция провалилась",
                          QMessageBox::Ok);
    break;
  }
}

void UserInteractionSystem::getMasterPassword(QString &pass) {
  pass =
      QInputDialog::getText(ParentWindow, "Мастер доступ",
                            "Введите пароль:", QLineEdit::Normal, "", nullptr);
}
