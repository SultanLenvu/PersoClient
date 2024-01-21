#ifndef TE310PRINTER_H
#define TE310PRINTER_H

#include <QHostInfo>

#include "General/definitions.h"
#include "isticker_printer.h"

/*! TE310 Printer interface
 * This class implements IStickerPrinter for TE310 printers through
 * TSC library functions
 */
class TE310Printer : public IStickerPrinter
{
  Q_OBJECT

 private:
  // Прототипы библиотечных функции
  //! TSC about() prototype
  typedef int (*TscAbout)(void);
  //! TSC openport() prototype
  typedef int (*TscOpenPort)(const char*);
  //! TSC sendcommand() prototype
  typedef int (*TscSendCommand)(const char*);
  //! TSC closeport() prototype
  typedef int (*TscClosePort)(void);

 private:
  //! Is logging enabled?
  bool LogEnable;
  //! Printer name
  QString Name;

  //! Is library loading failed?
  bool LibError;
  //! Path to TSC library
  QString TscLibPath;
  //! TSC Library itself
  QLibrary* TscLib;

  //! Last known parameters of the last sticker
  QHash<QString, QString> LastTransponderSticker;
  //! Last known parameters of the box sticker
  QHash<QString, QString> LastBoxSticker;
  //! Last known parameters of the pallet sticker
  QHash<QString, QString> LastPalletSticker;

  // Библиотечные функции
  //! points to about() function
  TscAbout about;
  //! points to openport() function
  TscOpenPort openPort;
  //! points to endcommand() function
  TscSendCommand sendCommand;
  //! points to closeport() function
  TscClosePort closePort;

 public:
  /*!
   * Construct the object, load settings and TSC library
   * \param parent QObject parent
   * \param name Printer name
   */
  explicit TE310Printer(QObject* parent, const QString& name);

  /*!
   * Validate configuration: check if TSC library path is correct,
   * and printers are available
   * \return true if everything is OK, false otherwise
   * \todo printers' (un)availability is not configuration issue
   */
  virtual bool checkConfiguration(void) override;

  /*!
   * Print sticker using parameters from map.
   * Memorize the configuration.
   * \param[in] parameters sticker parameters:
   * - issuer_sticker: Sticker issuer. Defines which function will
   *   be actually used
   *
   * \return operation status:
   * - ParameterError, if issuer name, S/N or PAN are empty, or issuer name is
   *   unknown (not NKD or ZSD)
   * - LibraryMissing: TSC Library is not loaded
   * - Completed: operation was completed successfully
   */
  virtual ReturnStatus printTransponderSticker(
      const QHash<QString, QString>* parameters) override;
  /*!
   * Print last sticker once more.
   * \return ParameterError if there's no known last configuration.
   * Otherwise returns what printTransponderSticker returns.
   */
  virtual ReturnStatus printLastTransponderSticker(void) override;

  /*!
   * Print box sticker
   * \param[in] parameters sticker parameters. Must contain following keys:
   * - id
   * - transponder_model
   * - quantity
   * - first_transponder_sn
   * - last_transponder_sn
   */
  virtual ReturnStatus printBoxSticker(
      const QHash<QString, QString>* parameters) override;
  /*!
   * Print last know box sticker
   * \sa printBoxSticker
   */
  virtual ReturnStatus printLastBoxSticker(void) override;

  /*!
   * Print pallet sticker
   * \param[in] parameters sticker parameters. Must contain following keys:
   * - id
   * - transponder_model
   * - quantity
   * - first_box_id
   * - last_box_id
   * - assembly_date
   */
  virtual ReturnStatus printPalletSticker(
      const QHash<QString, QString>* parameters) override;
  /*!
   * Print last know pallet sticker
   * \sa printPalletSticker
   */
  virtual ReturnStatus printLastPalletSticker(void) override;

  /*!
   * Execute TE310 script
   * \param[in] commandScript list of commands to execute
   * \return Completed
   * \todo why is this returning anything?
   */
  virtual ReturnStatus exec(const QStringList* commandScript) override;

  /*!
   * Load new settings
   * \sa loadSetting
   */
  virtual void applySetting(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(TE310Printer);
  //! \endcond
  /*!
   * Load settings from QSettings, namely:
   * - log_system/global_enable: whether to log events
   * - te310_printer/library_path: path to TSC library
   */
  void loadSetting(void);
  /*!
   * Send log line to a logger if logging is enabled
   * \param[in] log A new log line
   */
  void sendLog(const QString& log);
  /*!
   * Load TSC library and resolve needed symbols:
   * - about, stored at about
   * - openport, stored at openPort
   * - sendcommand, stored at sendCommand
   * - closeport, stored at closePort
   */
  bool loadTscLib(void);
  /*!
   * Print sticker for "NKD" issuer.
   * \param[in] parameters map containing what will be printer:
   * - PAN
   * - Manufacturer ID
   * - Battery insertion date
   * - Serial Number
   */
  void printNkdSticker(const QHash<QString, QString>* parameters);
  /*!
   * Print sticker for "ZKD" issuer.
   * \param[in] parameters map containing what will be printer:
   * - PAN
   * - Manufacturer ID
   * - Battery insertion date
   * - Serial Number
   */
  void printZsdSticker(const QHash<QString, QString>* parameters);
};

#endif  // TE310PRINTER_H
