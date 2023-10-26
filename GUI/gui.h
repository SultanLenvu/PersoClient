#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QSettings>
#include <QtWidgets>

/*!
 * GUI ADT
 */
class GUI : public QWidget {
 Q_OBJECT
 public:
  /*! GUI subclass type enumeration */
  enum GuiType { Authorization, Master, Production };

 public:
  //! \cond
  QHBoxLayout* MainLayout;
  //! \endcond

 private:
  /*! Type of instance of this class */
  GuiType Type;

 public:
  /*!
   * Constructs main layout
   * \param[in] parent GUI's parent
   * \param[in] type GUI type. Passed by subclass constructors
   */
  GUI(QWidget* parent, GuiType type);
  /*!
   * Default virtual destructor
   */
  virtual ~GUI();

  /*!
   * Create widgets, etc
   */
  virtual void create(void) = 0;
  /*!
   * Resize size to match contents
   */
  virtual void update(void) = 0;

  /*!
   * Get this GUI type
   */
  GuiType type(void);

 private:
  //! \cond
  Q_DISABLE_COPY(GUI);
  //! \endcond

 signals:
};

#endif  // GUI_H
