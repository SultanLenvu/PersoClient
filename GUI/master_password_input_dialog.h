#ifndef MasterPasswordInputDialog_H
#define MasterPasswordInputDialog_H

#include "input_dialog.h"

/*!
 * Master password dialog
 */
class MasterPasswordInputDialog : public InputDialog {
  Q_OBJECT
 private:
  //! \cond
  QSize DesktopGeometry;
  QVBoxLayout* MainLayout;

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
   * Construct object, Initialize dialog layout
   * \param[in] parent Parent window
   */
  explicit MasterPasswordInputDialog(QWidget* parent);
  /*!
   * Default destructor
   */
  ~MasterPasswordInputDialog();

  /*!
   * Store entered password
   * \param data Map where password will be stored
   */
  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  //! \cond
  Q_DISABLE_COPY(MasterPasswordInputDialog);
  //! \endcond
  /*!
   * Initialize layout
   */
  void create(void);
};

#endif  // MasterPasswordInputDialog_H
