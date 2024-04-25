#ifndef AUTHORIZATIONUSERINTERFACE_H
#define AUTHORIZATIONUSERINTERFACE_H

#include <types.h>
#include <QtWidgets>

class AuthorizationUserInterface : public QWidget {
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
  explicit AuthorizationUserInterface(QWidget* parent = nullptr);
  ~AuthorizationUserInterface() = default;
  Q_DISABLE_COPY_MOVE(AuthorizationUserInterface);

 private:
  void connectInternals(void);

 private:
  void create(void);
  void createAuthorizationGroup(void);

 private slots:
  void logOn(void);

 signals:
  void logOn_trigger(const StringDictionary& param);
};

#endif  // AUTHORIZATIONUSERINTERFACE_H
