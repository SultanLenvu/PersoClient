#ifndef MasterPasswordInputDialog_H
#define MasterPasswordInputDialog_H

#include "abstract_input_dialog.h"

class MasterPasswordInputDialog : public AbstractInputDialog {
  Q_OBJECT
 private:
  QSize DesktopGeometry;
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
  ~MasterPasswordInputDialog() = default;

  // AbstractInputDialog interface
 public:
  virtual void getData(StringDictionary& data) const override;

 private:
  Q_DISABLE_COPY_MOVE(MasterPasswordInputDialog);
  void create(void);
};

#endif  // MasterPasswordInputDialog_H
