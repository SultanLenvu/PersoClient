#include "authorization_user_interface.h"
#include "authorization_gui_subkernel.h"
#include "global_environment.h"

AuthorizationUserInterface::AuthorizationUserInterface(QWidget* parent)
    : QWidget(parent) {
  create();
  connectDependecies();
}

void AuthorizationUserInterface::logOn() {}

void AuthorizationUserInterface::connectDependecies() {
  AuthorizationGuiSubkernel* ags =
      GlobalEnvironment::instance()->getObject<AuthorizationGuiSubkernel>(
          "AuthorizationGuiSubkernel");

  connect(AuthorizePushButton, &QPushButton::clicked, ags,
          &AuthorizationGuiSubkernel::logOn);
}

void AuthorizationUserInterface::create() {
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
