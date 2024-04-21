#ifndef MASTERACCESSDIALOG_H
#define MASTERACCESSDIALOG_H

#include "abstract_input_dialog.h"

class MasterAccessDialog : public AbstractInputDialog {
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
  explicit MasterAccessDialog(QWidget* parent);
  ~MasterAccessDialog() = default;

  // AbstractInputDialog interface
 public:
  virtual void getData(StringDictionary& data) const override;

 private:
  Q_DISABLE_COPY_MOVE(MasterAccessDialog);
  void create(void);
};

#endif  // MASTERACCESSDIALOG_H
