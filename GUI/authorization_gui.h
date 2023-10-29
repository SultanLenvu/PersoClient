#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include "abstract_gui.h"

class AuthorizationGUI : public AbstractGUI {
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
  explicit AuthorizationGUI(QWidget* parent);
  ~AuthorizationGUI();

  virtual void update(void) override;

 private:
  Q_DISABLE_COPY(AuthorizationGUI);
  void create(void);
  void createAuthorizationGroup(void);

 private slots:
  void on_ModeChoiceCurrentTextChanged_slot(const QString& tex);
};

#endif  // AUTHORIZATIONGUI_H
