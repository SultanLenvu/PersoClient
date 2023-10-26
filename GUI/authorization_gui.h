#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include "gui.h"

/*!
 * Authorization GUI mode and its widgets
 */
class AuthorizationGUI : public GUI {
 public:
  //! \cond
  QGroupBox* ControlPanelGroup;
  QGridLayout* ControlPanelLayout;

  QLabel* LoginLabel;
  QLineEdit* LoginLineEdit;
  QLabel* PasswordLabel;
  QLineEdit* PasswordLineEdit;
  QPushButton* AuthorizePushButton;
  QSpacerItem* ControlPanelVS;
  //! \endcond

 public:
  /*!
   * Default constructor
   * \param[in] parent Window parent
   */
  AuthorizationGUI(QWidget* parent);

  /*!
   * Create layout
   */
  virtual void create(void) override;
  /*!
   * No-op
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(AuthorizationGUI);
  //! \endcond
};

#endif // AUTHORIZATIONGUI_H
