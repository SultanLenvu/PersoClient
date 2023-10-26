#ifndef INTERACTION_SYSTEM_H
#define INTERACTION_SYSTEM_H

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QProgressBar>
#include <QProgressDialog>

#include "authorization_dialog.h"
#include "custom_transponder_sticker_dialog.h"
#include "transponder_sticker_scan_dialog.h"

/*!
 * Interaction system singleton. Show dialogs, messages, etc
 */
class InteractionSystem : public QWidget {
 Q_OBJECT

 private:
  //! Is logging enabled
  bool LogEnable;
  //! Shows operation progress
  QProgressDialog* ProgressDialog;
  //! ProgressDialog fill percentage
  uint32_t CurrentOperationStep;

  //! Maximal duration timer
  QTimer* ODTimer;
  //! Quant timer
  QTimer* ODQTimer;
  //! Timer to measure elapsed time
  QElapsedTimer* ODMeter;

 public:
  /*!
   * Get instance of this class.
   */
  static InteractionSystem* instance(void);

 public slots:
  /*!
   * Show information message box
   * \param[in] data message to show
   */
  void generateMessage(const QString& data);
  /*!
   * Show error message box
   * \param[in] text message to show
   */
  void generateErrorMessage(const QString& text);

  /*!
   * Create progress dialog with createProgressDialog,
   * read quant duration, start ODQTimer with that duration,
   * and start ODTimer and ODMeter 
   */
  void startOperationProgressDialog(const QString& operationName);
  /*!
   * Close progress dialog window and record operation time
   */
  void finishOperationProgressDialog(const QString& operationName);

  /*!
   * Ask user for password to the master interface
   * \param[out] pass where to store entered password
   */
  void getMasterPassword(QString& pass);
  /*!
   * Ask user for authorization data
   * \param[out] data where to store authorization data
   */
  void getAuthorizationData(QMap<QString, QString>* data);
  /*!
   * Ask user for sticker data via TransponderStickerScanDialog
   * \param[out] data where to store sticker data
   * \param[out] ok was data saved or did user cancel this operation
   */
  void getTransponderStickerData(QStringList* data, bool& ok);
  /*!
   * Ask user for custom sticker data via CustomStickerScanDialog
   * \param[out] data where to store sticker data
   * \param[out] ok was data saved or did user cancel this operation
   */
  void getCustomTransponderStickerData(QMap<QString, QString>* data, bool& ok);

  /*!
   * Load settings using loadSettings method
   */
  void applySettings(void);

 private:
  /*!
   * Construct object, set object name, load settings and
   * initialize timers
   * \param[in] window Parent window
   */
  explicit InteractionSystem(QWidget* window);
  //! \cond
  Q_DISABLE_COPY(InteractionSystem)
  //! \endcond
  /*!
   * Load settings from QSettings, namely:
   * - user_interaction_system/log_enable: whether logging is enabled
   */
  void loadSettings(void);
  /*!
   * Send log line to the logger if logging is enabled
   * \param[in] log new log line
   */
  void sendLog(const QString& log);

  /*!
   * Construct QPRogressDialog and store it in ProgressDialog
   */
  void createProgressDialog(void);
  /*!
   * Destroy ProgressDialog
   */
  void destroyProgressDialog(void);
  /*!
   * Construct timers:
   * - ODTimer to track timeouts
   * - ODMeter to measure operation execution time
   * - ODQTimer to tick on quants
   */
  void createTimers(void);

 private slots:
  /*!
   * Timeout, stop the operation
   */
  void on_ODTimerTimeout_slot(void);
  /*!
   * Quantum timer ticked, show some progress on progress dialog
   */
  void on_ODQTimerTimeout_slot(void);

 signals:
  /*!
   * Emitted to send a log line to the logger
   * \param[in] log the new log line
   */
  void logging(const QString& log);
  /*!
   * Emitted to cancel any ongoing operations
   */
  void abortCurrentOperation(void);
};

#endif  // INTERACTION_SYSTEM_H
