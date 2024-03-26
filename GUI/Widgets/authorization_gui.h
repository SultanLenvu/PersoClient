#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include <QtWidgets>

class AuthorizationGui : public QWidget {
  Q_OBJECT

 private:
  QSettings Settings;
  QRect DesktopGeometry;

 public:
  QHBoxLayout* MainLayout;
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
  ~AuthorizationGui() = default;

 private slots:
  void logOn_guiSlot(void);

 private:
  Q_DISABLE_COPY(AuthorizationGui);
  void connectDependecies(void);

 private:
  void create(void);
  void createAuthorizationGroup(void);
};

#endif  // AUTHORIZATIONGUI_H
