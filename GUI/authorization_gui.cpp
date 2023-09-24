#include "authorization_gui.h"

AuthorizationGUI::AuthorizationGUI(QWidget* parent)
    : GUI(parent, Authorization) {}

void AuthorizationGUI::create() {
  QSettings settings;

  ControlPanelGroup = new QGroupBox("Авторизация");
  ControlPanelGroup->setAlignment(Qt::AlignCenter);
  MainLayout->addWidget(ControlPanelGroup);

  ControlPanelLayout = new QGridLayout();
  ControlPanelGroup->setLayout(ControlPanelLayout);

  LoginLabel = new QLabel("Логин");
  ControlPanelLayout->addWidget(LoginLabel, 0, 0, 1, 1);
  LoginLineEdit =
      new QLineEdit(settings.value("Authorization/Login").toString());
  ControlPanelLayout->addWidget(LoginLineEdit, 0, 1, 1, 1);

  PasswordLabel = new QLabel("Пароль");
  ControlPanelLayout->addWidget(PasswordLabel, 1, 0, 1, 1);
  PasswordLineEdit = new QLineEdit();
  PasswordLineEdit->setEchoMode(QLineEdit::Password);
  ControlPanelLayout->addWidget(PasswordLineEdit, 1, 1, 1, 1);

  AuthorizePushButton = new QPushButton("Войти");
  ControlPanelLayout->addWidget(AuthorizePushButton, 2, 0, 1, 2);

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  ControlPanelLayout->addItem(ControlPanelVS, 3, 0, 1, 1);
}

void AuthorizationGUI::update() {}
