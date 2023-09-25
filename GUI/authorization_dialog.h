#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

#include <QDialog>
#include <QtWidgets>

class AuthorizationDialog : public QDialog {
  Q_OBJECT

 private:
  QRect DesktopGeometry;
  QVBoxLayout* MainLayout;

  QHBoxLayout* LoginLayout;
  QLabel* LoginLabel;
  QLineEdit* LoginLineEdit;
  QHBoxLayout* PasswordLayout;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;

  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  AuthorizationDialog(QWidget* parent);
  void getData(QMap<QString, QString>* data);
};
#endif // AUTHORIZATIONDIALOG_H
