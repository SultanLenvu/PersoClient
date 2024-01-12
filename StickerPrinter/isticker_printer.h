#ifndef ISTICKERPRINTER_H
#define ISTICKERPRINTER_H

#include <QHash>
#include <QLibrary>
#include <QObject>
#include <QSettings>
#include <QtPrintSupport/QPrinterInfo>

/*! \brief Sticker printer interface
 */
class IStickerPrinter : public QObject
{
  Q_OBJECT
 public:
  /*! Subclass type
   */
  enum StickerPrinterType
  {
    Unknown,
    TE310,
  };
  //! \cond
  Q_ENUM(StickerPrinterType);
  //! \endcond

  /*! Sticker Printer operations return statuses
   */
  enum ReturnStatus
  {
    ParameterError,
    LibraryMissed,
    ConnectionError,
    Failed,
    Completed,
  };
  //! \cond
  Q_ENUM(ReturnStatus);
  //! \endcond

 protected:
  /*! Which printer is this */
  StickerPrinterType Type;

 public:
  /*!
   * Constructs object, sets name and type
   * \param[in] type subclass type, passed by subclass constructor
   * \param[in] parent QObject parent
   */
  IStickerPrinter(QObject* parent, StickerPrinterType type);
  /*!
   * Default destructor
   */
  virtual ~IStickerPrinter();

  /*!
   * Validate printer configuration
   * \return true if configuration is correct, false otherwise
   */
  virtual bool checkConfiguration(void) = 0;

  /*!
   * Print sticker according to parameters and memorize them
   * \param[in] parameters parameters used for sticker
   * \return Operation status
   */
  virtual ReturnStatus printTransponderSticker(
      const QHash<QString, QString>* parameters) = 0;
  /*!
   * Print sticker according to memorized parameters
   * \return Operation status
   */
  virtual ReturnStatus printLastTransponderSticker(void) = 0;

  /*!
   * Print box sticker
   * \param parameters sticker parameters
   */
  virtual ReturnStatus printBoxSticker(
      const QHash<QString, QString>* parameters) = 0;
  /*!
   * Print box sticker with last know parameters
   */
  virtual ReturnStatus printLastBoxSticker(void) = 0;

  /*!
   * Print pallet sticker
   * \param parameters sticker parameters
   */
  virtual ReturnStatus printPalletSticker(
      const QHash<QString, QString>* parameters) = 0;
  /*!
   * Print pallet sticker with last know parameters
   */
  virtual ReturnStatus printLastPalletSticker(void) = 0;

  /*!
   * Execute a printer script
   * \param[in] commandScript list of commands to execute
   */
  virtual ReturnStatus exec(const QStringList* commandScript) = 0;

  /*!
   * Load printer settings
   */
  virtual void applySetting(void) = 0;

 private:
  //! \cond
  Q_DISABLE_COPY(IStickerPrinter);
  //! \endcond

 signals:
  /*!
   * Emitted to logger with a log line
   */
  void logging(const QString& log);
};

#endif  // ISTICKERPRINTER_H
