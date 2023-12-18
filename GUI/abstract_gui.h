#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QSettings>
#include <QtWidgets>

/*!
 * GUI ADT
 */
class AbstractGui : public QWidget {
  Q_OBJECT
 public:
  /*!
   * GUI subclass types
   */
  enum GuiType { Authorization, Master, Production, Testing };

 public:
  /*!
   * Layout which hosts everything
   */
  QHBoxLayout* MainLayout;

 private:
  /*!
   * Subclass type
   */
  GuiType Type;

 public:
  /*!
   * Construct GUI. Initialize MainLayout
   * \param[in] parent QObject parent
   * \param[in] type Subclass type. Should be set by subclass
   * constructor
   */
  AbstractGui(QWidget* parent, GuiType type);
  /*!
   * Default destructor
   */
  virtual ~AbstractGui();

  /*!
   * Update GUI geometry
   */
  virtual void update(void) = 0;

  /*!
   * Get GUI subclass type
   * \return GUI subclass type
   */
  GuiType type(void);

 private:
  //! \cond
  Q_DISABLE_COPY(AbstractGui);
  //! \endcond

 signals:
  /*!
   * Emitted if visibility has changed
   */
  void visibilityChanged(void);
};

#endif  // ABSTRACT_GUI_H
