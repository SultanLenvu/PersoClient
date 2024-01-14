#include "authorization_gui.h"

AuthorizationGui::AuthorizationGui(QWidget* parent) : AbstractGui(parent) {
  create();
}

AuthorizationGui::~AuthorizationGui() {
  //  qDebug() << "Deleted.";
}

void AuthorizationGui::update() {}

AbstractGui::GuiType AuthorizationGui::type() {
  return Authorization;
}

void AuthorizationGui::create() {
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

void AuthorizationGui::createAuthorizationGroup() {
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
