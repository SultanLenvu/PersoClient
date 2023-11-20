#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QThread>

#include "interface_programmer.h"

/*!
 * \brief JLink.exe programmer
 */
class JLinkExeProgrammer : public IProgrammer {
 Q_OBJECT
 private:
  //! is logging enabled
  bool LogEnable;
  //! is JLink output enabled
  bool ExtendedLoggingEnable;
  //! Speed to pass to JLink
  uint32_t Speed;
  //! Pointer to JLink process
  QProcess *JLinkProcess;

  /*! File to write JLink script to
   * TODO why not pipes and writing the script to stdin?
   */
  QFile *JLinkScript;

  //! Arguments for JLink
  QStringList ProcessArguments;

  //! JLink output
  QStringList ProcessOutput;

 public:
  /*!
   * Default contructor
   * \param[in] parent QObject parent
   */
  explicit JLinkExeProgrammer(QObject* parent);
  /*!
   * Default destructor
   */
  ~JLinkExeProgrammer();

 public:
  /*!
   * Read UCID using 'mem' command
   * \param[out] ucid String to put UCID into
   */
  virtual ReturnStatus getUcid(QString* ucid) override;
  /*!
   * Load firmware to device.
   * \param[in] firmware File containing firmware
   */
  virtual ReturnStatus loadFirmware(QFile* firmware) override;
  /*!
   * Unlock device flash memory and load firmware to it.
   * \param[in] firmware File containing firmware
   */
  virtual ReturnStatus loadFirmwareWithUnlock(QFile* firmware) override;
  /*!
   * Read device firmware to saved_firmware.bin
   */
  virtual ReturnStatus readFirmware(void) override;
  /*!
   * Erase device firmware
   */
  virtual ReturnStatus eraseFirmware(void) override;

  /*!
   * Read user data from device to saved_user_data.bin
   */
  virtual ReturnStatus readData(void) override;
  /*!
   * Load user data onto device
   * \param[in] data File to read data from
   */
  virtual ReturnStatus loadData(QFile* data) override;

  /*!
   * Unlock device flash memory
   */
  virtual ReturnStatus unlockDevice(void) override;
  /*!
   * Lock device flash memory
   */
  virtual ReturnStatus lockDevice(void) override;

  /*!
   * Load settings from QSettings
   * \sa loadSettings
   */
  virtual void applySettings() override;

 private:
  //! \cond
  Q_DISABLE_COPY(JLinkExeProgrammer);
  //! \endcond
  /*!
   * Send log line if logging is enabled.
   * \param[in] log log line
   */
  void sendLog(const QString& log);
  /*!
   * Load settings from QSettings. This includes JLink.exe path,
   * speed and whether to log JLink.exe output
   */
  void loadSettings(void);
  /*!
   * Finish constructing JLink script and run JLink.exe
   * with that script.
   * If verbose logging is enabled, full JLink.exe output will
   * be logged.
   */
  void executeJLinkScript(void);
  /*!
   * Create file named JLINK_COMMAND_SCRIPT_DEFAULT_NAME
   * and add some initial commands to it.
   */
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
