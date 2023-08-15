#include "user_Interaction_system.h"

UserInteractionSystem::UserInteractionSystem(QObject *parent, QWidget *window)
    : QObject(parent) {
  ParentWindow = window;
}

void UserInteractionSystem::generateMessage(const QString& data) {
  QMessageBox::information(ParentWindow, "Сообщение", data, QMessageBox::Ok);
}

void UserInteractionSystem::getMasterPassword(QString& pass) {
  pass =
      QInputDialog::getText(ParentWindow, "Мастер доступ",
                            "Введите пароль:", QLineEdit::Normal, "", nullptr);
}

void UserInteractionSystem::generateErrorMessage(const QString &text) {
  QMessageBox::critical(ParentWindow, "Ошибка", text, QMessageBox::Ok);
}
