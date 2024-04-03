#ifndef AUTHORIZATIONUSERINTERFACE_H
#define AUTHORIZATIONUSERINTERFACE_H

#include <QtWidgets>
#include "types.h"

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

 private:
  Q_DISABLE_COPY_MOVE(AuthorizationUserInterface);
  void connectDependecies(void);

 private:
  void create(void);
  void createAuthorizationGroup(void);

 private slots:
  void logOn(void);

 signals:
  void logOn_signal(const StringDictionary& param);
};

#endif  // AUTHORIZATIONUSERINTERFACE_H
