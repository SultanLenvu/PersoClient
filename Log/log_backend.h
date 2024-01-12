#ifndef LOGBACKEND_H
#define LOGBACKEND_H

#include <QObject>
#include <QSettings>

/*!
 * \brief logging backend interface
 *
 * Provides an interface for logging backends, which write or display log
 * lines. These backends are meant to be integrated with Qt metaobject system
 * so they can send signals and have slots.
 */
class LogBackend : public QObject
{
  Q_OBJECT

 public:
  /*! Default constructor
   */
  explicit LogBackend(QObject* parent);
  /*! Default destuctor
   */
  virtual ~LogBackend();

  /*!
   * Writes or displays log line
   * \param[in] dest log line to write
   */
  virtual void writeLogLine(const QString& str) = 0;
  /*!
   * Clears output. Useful for backends which display logs on the screen.
   */
  virtual void clear(void) = 0;
  /*!
   * Load new settings from QSettings
   */
  virtual void applySettings(void) = 0;

 private:
  //! \cond
  Q_DISABLE_COPY(LogBackend);
  //! \endcond
};

#endif /* LOGBACKEND_H */
