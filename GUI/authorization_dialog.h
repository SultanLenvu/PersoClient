#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QtWidgets>
#include <QSize>

#include "abstract_input_dialog.h"

/*!
 * Dialog for authorization data
 */
class AuthorizationDialog : public InputDialog {
  Q_OBJECT

 private:
  //! \cond
  QSize DesktopGeometry;
  QVBoxLayout* MainLayout;

  QHBoxLayout* LoginLayout;
  QLabel* LoginLabel;
  QLineEdit* LoginLineEdit;
  QHBoxLayout* PasswordLayout;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;

  QHBoxLayout* ButtonLayout;
  QPushButton* OkButton;
  QPushButton* CancelButton;

  QSpacerItem* MainLayoutVS;
  //! \endcond

 public:
  /*!
   * Construct class and create layout
   * \param[in] parent QObject parent
   */
  explicit AuthorizationDialog(QWidget* parent);
  /*!
   * Default destructor
   */
  ~AuthorizationDialog();

  /*!
   * Put contents of login and password fields into data
   * \param[out] data map to store login and password
   */
  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  //! \cond
  Q_DISABLE_COPY(AuthorizationDialog);
  //! \endcond
  /*!
   * Initialize dialog layout
   */
  void create(void);
};
#endif  // AUTHORIZATIONDIALOG_H
