#include "authorization_user_interface.h"
#include "global_environment.h"
#include "production_manager_gui_subkernel.h"

AuthorizationUserInterface::AuthorizationUserInterface(QWidget* parent)
    : QWidget(parent) {
  create();
  connectInternals();
}

void AuthorizationUserInterface::connectInternals() {
  connect(AuthorizePushButton, &QPushButton::clicked, this,
          &AuthorizationUserInterface::logOn);
}

void AuthorizationUserInterface::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  GeneralLayout = new QVBoxLayout();
  MainLayout->addLayout(GeneralLayout);

  ModeChoiceLayout = new QHBoxLayout();
  GeneralLayout->addLayout(ModeChoiceLayout);

  ModeChoiceLabel = new QLabel("Режим работы");
  ModeChoiceLayout->addWidget(ModeChoiceLabel);
  ModeChoice = new QComboBox();
  ModeChoice->addItem("Сборка");
  ModeChoice->addItem("Тестирование");
  GeneralLayout->addWidget(ModeChoice);
  ModeChoice->setCurrentIndex(0);

  createAuthorizationGroup();

  AuthorizePushButton = new QPushButton("Войти");
  GeneralLayout->addWidget(AuthorizePushButton);
  connect(AuthorizePushButton, &QPushButton::clicked, this,
          &AuthorizationUserInterface::logOn);

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  GeneralLayout->addItem(ControlPanelVS);
}

void AuthorizationUserInterface::createAuthorizationGroup() {
  AuthorizationGroup = new QGroupBox("Авторизация");
  AuthorizationGroup->setAlignment(Qt::AlignCenter);
  GeneralLayout->insertWidget(GeneralLayout->indexOf(ControlPanelVS) - 1,
                              AuthorizationGroup);

  AuthorizationLayout = new QVBoxLayout();
  AuthorizationGroup->setLayout(AuthorizationLayout);

  LoginLayout = new QHBoxLayout();
  AuthorizationLayout->addLayout(LoginLayout);

  LoginLabel = new QLabel("Логин");
  LoginLayout->addWidget(LoginLabel);
  LoginLineEdit =
      new QLineEdit(Settings.value("Authorization/Login").toString());
  LoginLayout->addWidget(LoginLineEdit);

  PasswordLayout = new QHBoxLayout();
  AuthorizationLayout->addLayout(PasswordLayout);

  PasswordLabel = new QLabel("Пароль");
  PasswordLayout->addWidget(PasswordLabel);
  PasswordLineEdit = new QLineEdit();
  PasswordLineEdit->setEchoMode(QLineEdit::Password);
  PasswordLayout->addWidget(PasswordLineEdit);
}

void AuthorizationUserInterface::logOn() {
  StringDictionary param;
  param.insert("login", LoginLineEdit->text());
  param.insert("password", PasswordLineEdit->text());

  emit logOn_trigger(param);
}
