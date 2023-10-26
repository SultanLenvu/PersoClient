#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

#include <QDialog>
#include <QtWidgets>

/*!
 * Dialog for authorization data
 */
class AuthorizationDialog : public QDialog {
 Q_OBJECT

 private:
  //! \cond
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
  //! \endcond

 public:
  /*!
   * Construct class and create layout
   */
  AuthorizationDialog(QWidget* parent);
  /*!
   * Put contents of login and password fields into data
   * \param[out] data map to store login and password
   */
  void getData(QMap<QString, QString>* data);

 private:
  Q_DISABLE_COPY(AuthorizationDialog);
};
#endif // AUTHORIZATIONDIALOG_H
