#include "user_Interaction_system.h"

UserInteractionSystem::UserInteractionSystem(QObject *parent, QWidget *window)
    : QObject(parent) {
  ParentWindow = window;
}

void UserInteractionSystem::firmwareManagerNotification(
    ProgrammatorManager::PerfomingStatus status) {
  switch (status) {
  case ProgrammatorManager::Completed:
    QMessageBox::information(ParentWindow, "Менеджер",
                             "Операция успешно выполнена", QMessageBox::Ok);
    break;
  case ProgrammatorManager::Failed:
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

void UserInteractionSystem::generateErrorMessage(const QString &text) {
  QMessageBox::critical(ParentWindow, "Доступ", text, QMessageBox::Ok);
}
