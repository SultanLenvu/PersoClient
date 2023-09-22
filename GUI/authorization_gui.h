#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include "gui.h"

class AuthorizationGUI : public GUI {
 public:
  QGroupBox* ControlPanelGroup;
  QGridLayout* ControlPanelLayout;

  QLabel* LoginLabel;
  QLineEdit* LoginLineEdit;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;
  QPushButton* AuthorizePushButton;
  QSpacerItem* ControlPanelVS;

 public:
  AuthorizationGUI(QWidget* parent);

  virtual void create(void) override;
  virtual void update(void) override;
};

#endif // AUTHORIZATIONGUI_H
