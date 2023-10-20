#include "authorization_dialog.h"

AuthorizationDialog::AuthorizationDialog(QWidget* parent) : QDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5,
              DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1);
  setWindowTitle("Авторизация");

  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  LoginLayout = new QHBoxLayout();
  MainLayout->addLayout(LoginLayout);
  LoginLabel = new QLabel("Логин");
  LoginLayout->addWidget(LoginLabel);
  LoginLineEdit = new QLineEdit();
  LoginLayout->addWidget(LoginLineEdit);

  PasswordLayout = new QHBoxLayout();
  MainLayout->addLayout(PasswordLayout);
  PasswordLabel = new QLabel("Логин");
  PasswordLayout->addWidget(PasswordLabel);
  PasswordLineEdit = new QLineEdit();
  PasswordLayout->addWidget(PasswordLineEdit);

  OkButton = new QPushButton("Авторизоваться");
  MainLayout->addWidget(OkButton);
  connect(OkButton, &QPushButton::clicked, this, &QDialog::accept);

  CancelButton = new QPushButton("Отмена");
  MainLayout->addWidget(CancelButton);
  connect(CancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void AuthorizationDialog::getData(QHash<QString, QString>* data) {
  if (!data) {
    return;
  }

  data->insert("login", LoginLineEdit->text());
  data->insert("password", PasswordLineEdit->text());
}
