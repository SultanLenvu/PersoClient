#ifndef AUTHORIZATIONGUI_H
#define AUTHORIZATIONGUI_H

#include "abstract_gui.h"

/*!
 * Authorization GUI mode and its widgets
 */
class AuthorizationGUI : public AbstractGUI
{
  Q_OBJECT

 private:
  //! Client settings
  QSettings Settings;
  //! Screen geometry
  QRect DesktopGeometry;

 public:
  //! \cond
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
  //! \endcond

 public:
  /*!
   * Default constructor
   * \param[in] parent Window parent
   */
  explicit AuthorizationGUI(QWidget* parent);
  /*!
   * Default destructor
   */
  ~AuthorizationGUI();

  /*!
   * No-op
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(AuthorizationGUI);
  //! \endcond
  /*!
   * Initialize layout
   */
  void create(void);
  /*!
   * Initialize Authorization group widgets
   */
  void createAuthorizationGroup(void);

 private slots:
  /*!
   * Removes or adds authorization group depending on string passed
   * \param[in] text: group is removed if it is "Тестирование", and
   * added otherwise
   */
  void on_ModeChoiceCurrentTextChanged_slot(const QString& text);
};

#endif  // AUTHORIZATIONGUI_H
