#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include "abstract_gui.h"

class AuthorizationGui : public AbstractGui {
  Q_OBJECT

 private:
  QSettings Settings;
  QRect DesktopGeometry;

 public:
  QVBoxLayout* GeneralLayout;

  QHBoxLayout* ModeChoiceLayout;
  QLabel* ModeChoiceLabel;
  QComboBox* ModeChoice;

  QGroupBox* AuthorizationGroup;
  QVBoxLayout* AuthorizationLayout;

  QHBoxLayout* LoginLayout;
  QLabel* LoginLabel;
  QLineEdit* LoginLineEdit;

  QHBoxLayout* PasswordLayout;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;

  QPushButton* AuthorizePushButton;

  QSpacerItem* ControlPanelVS;

 public:
  explicit AuthorizationGui(QWidget* parent);
  ~AuthorizationGui();

  virtual void updateModelViews(void) override;
  virtual GuiType type(void) override;

 private:
  Q_DISABLE_COPY(AuthorizationGui);
  void create(void);
  void createAuthorizationGroup(void);
};

#endif  // AUTHORIZATIONGUI_H
