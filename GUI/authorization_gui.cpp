#include "authorization_gui.h"

AuthorizationGUI::AuthorizationGUI(QWidget* parent)
    : AbstractGUI(parent, Authorization)
{
  create();
}

AuthorizationGUI::~AuthorizationGUI()
{
  //  qDebug() << "Deleted.";
}

void AuthorizationGUI::update() {}

void AuthorizationGUI::create()
{
  GeneralLayout = new QVBoxLayout();
  MainLayout->addLayout(GeneralLayout);

  ModeChoiceLayout = new QHBoxLayout();
  GeneralLayout->addLayout(ModeChoiceLayout);

  ModeChoiceLabel = new QLabel("Режим работы");
  ModeChoiceLayout->addWidget(ModeChoiceLabel);
  ModeChoice = new QComboBox();
  ModeChoice->addItem("Производство");
  ModeChoice->addItem("Тестирование");
  GeneralLayout->addWidget(ModeChoice);
  connect(ModeChoice, &QComboBox::currentTextChanged, this,
          &AuthorizationGUI::on_ModeChoiceCurrentTextChanged_slot);
  ModeChoice->setCurrentIndex(0);

  createAuthorizationGroup();

  AuthorizePushButton = new QPushButton("Войти");
  GeneralLayout->addWidget(AuthorizePushButton);

  ControlPanelVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  GeneralLayout->addItem(ControlPanelVS);
}

void AuthorizationGUI::createAuthorizationGroup()
{
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

void AuthorizationGUI::on_ModeChoiceCurrentTextChanged_slot(const QString& text)
{
  if (text == "Тестирование") {
    GeneralLayout->removeWidget(AuthorizationGroup);
    delete AuthorizationGroup;
  } else {
    createAuthorizationGroup();
  }

  adjustSize();
  emit visibilityChanged();
}
