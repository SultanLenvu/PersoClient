#include "master_access_dialog.h"

MasterAccessDialog::MasterAccessDialog(QWidget* parent)
    : AbstractInputDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::primaryScreen()->size();

  create();
  adjustSize();
  setFixedSize(size());

  setWindowTitle("Мастер доступ");
}

void MasterAccessDialog::getData(StringDictionary& data) const {
  data.insert("password", PasswordLineEdit->text());
}

void MasterAccessDialog::create()
{
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

  //  MainLayoutVS =
  //      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  MainLayout->addItem(MainLayoutVS);
}
