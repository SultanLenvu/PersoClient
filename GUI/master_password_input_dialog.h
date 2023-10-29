#ifndef MasterPasswordInputDialog_H
#define MasterPasswordInputDialog_H

#include "input_dialog.h"

class MasterPasswordInputDialog : public InputDialog {
  Q_OBJECT
 private:
  QRect DesktopGeometry;
  QVBoxLayout* MainLayout;

  QHBoxLayout* PasswordLayout;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;

  QHBoxLayout* ButtonLayout;
  QPushButton* OkButton;
  QPushButton* CancelButton;

  QSpacerItem* MainLayoutVS;

 public:
  explicit MasterPasswordInputDialog(QWidget* parent);
  ~MasterPasswordInputDialog();

  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  Q_DISABLE_COPY(MasterPasswordInputDialog);
  void create(void);
};

#endif  // MasterPasswordInputDialog_H
