#include "master_password_input_dialog.h"

MasterPasswordInputDialog::MasterPasswordInputDialog(QWidget* parent)
    : InputDialog(parent, MasterPassword) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5,
              DesktopGeometry.width() * 0.15, DesktopGeometry.height() * 0.05);
  setWindowTitle("Авторизация");

  create();
}

MasterPasswordInputDialog::~MasterPasswordInputDialog() {}

void MasterPasswordInputDialog::getData(QHash<QString, QString>* data) const {
  if (!data) {
    return;
  }

  data->insert("password", PasswordLineEdit->text());
}

void MasterPasswordInputDialog::create() {
  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  PasswordLayout = new QHBoxLayout();
  MainLayout->addLayout(PasswordLayout);
  PasswordLabel = new QLabel("Пароль");
  PasswordLayout->addWidget(PasswordLabel);
  PasswordLineEdit = new QLineEdit();
  PasswordLineEdit->setEchoMode(QLineEdit::Password);
  PasswordLayout->addWidget(PasswordLineEdit);

  ButtonLayout = new QHBoxLayout();
  MainLayout->addLayout(ButtonLayout);

  OkButton = new QPushButton("Ок");
  ButtonLayout->addWidget(OkButton);
  connect(OkButton, &QPushButton::clicked, this, &QDialog::accept);

  CancelButton = new QPushButton("Отмена");
  ButtonLayout->addWidget(CancelButton);
  connect(CancelButton, &QPushButton::clicked, this, &QDialog::reject);

  MainLayoutVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  MainLayout->addItem(MainLayoutVS);
}
