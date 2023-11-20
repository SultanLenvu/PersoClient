#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QHostAddress>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QTime>
#include <QUdpSocket>

#include "Log/file_log_backend.h"
#include "Log/log_backend.h"
#include "Log/widget_log_backend.h"

/* Глобальная система логгирования */
//==================================================================================

/*!
 * \brief logging system
 *
 * Object of this class accept log lines through signals and pass them to one
 * or more backends.
 */
class LogSystem : public QObject {
  Q_OBJECT

 private:
  QString SavePath;
  QList<LogBackend*> Backends;
  WidgetLogBackend* WidgetLogger;
  FileLogBackend* FileLogger;

 public:
  /*! Default destructor 
   */
  ~LogSystem();
  /*!
   * Get widget logger instance
   * \return widget logger
   */
  WidgetLogBackend* getWidgetLogger();
  /*!
   * Get LogSystem instance
   * \return LogSystem itself
   */
  static LogSystem* instance(void);

 public slots:
  /*!
   * Request log backends to lear their outputs
   */
  void clear(void);
  /*!
   * Give log backends a new line to log
   * \param[in] log a new log line
   */
  void generate(const QString& log);

  /*!
   * Load settings from QSettings
   * \sa loadSettings
   */
  void applySettings(void);

 private:
  /*!
   * Default constructor
   * \param[in] parent QObject parent
   */
  LogSystem(QObject* parent);
  //! \cond
  Q_DISABLE_COPY(LogSystem)
  //! \endcond
  /*!
   * Load settings from QSettings, namely:
   * - log_system/udp_listen_enable: whether to listen UDP for log lines
   * - log_system/udp_listen_ip: IP to listen for log lines
   * - log_system/upd_listen_port: port to listen for log lines
   */
  void loadSettings(void);
};

#endif  // LOGSYSTEM_H
