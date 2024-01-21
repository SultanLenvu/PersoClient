#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QFileInfo>
#include <QMutex>
#include <QObject>
#include <QString>

#include "General/definitions.h"
#include "General/types.h"

/*!
 * \brief Programmer interface
 */
class IProgrammer : public QObject
{
  Q_OBJECT
 public:
  /*! Programmer operations return statuses
   */
  enum ReturnStatus
  {
    NotExecuted,
    FirmwareFileError,
    DataFileError,
    DriverMissing,
    ProgrammatorError,
    Completed
  };
  //! \cond
  Q_ENUM(ReturnStatus);
  //! \endcond

  /*! Subclass type
   */
  enum ProgrammerType
  {
    JLink
  };
  //! \cond
  Q_ENUM(ProgrammerType);
  //! \endcond

 protected:
  //! Subclass type
  ProgrammerType Type;

 public:
  /*!
   * Sets object name and type
   * \param[in] parent parent
   * \param[in] type type passed by subclass constructor
   */
  explicit IProgrammer(QObject* parent, ProgrammerType type);
  /*!
   * Default destructor
   */
  virtual ~IProgrammer();

  /*! Get class type
   * \return subclass type
   * \todo why it isn't virtual
   */
  ProgrammerType type() const;

 public:
  /*!
   * Get device UCID
   * \param[out] ucid where to store UCID
   * \return operation status
   */
  virtual ReturnStatus getUcid(QString* ucid) = 0;
  /*!
   * Load firmware from file to device
   * \param[in] firmware file with firmware
   * \return operation status
   */
  virtual ReturnStatus loadFirmware(QFile* firmware) = 0;
  /*!
   * Unlock device memory and load firmware from file to device
   * \param[in] firmware file with firmware
   * \return operation status
   */
  virtual ReturnStatus loadFirmwareWithUnlock(QFile* firmware) = 0;
  /*!
   * Read firmware from device to implementation-defined location
   * \return operation status
   */
  virtual ReturnStatus readFirmware(void) = 0;
  /*!
   * Erase firmware from the file
   * \return operation status
   */
  virtual ReturnStatus eraseFirmware(void) = 0;

  /*!
   * Load user data from file to device
   * \param[in] data file containing data
   */
  virtual ReturnStatus loadData(QFile* data) = 0;
  /*!
   * Read user data from device to implementation-defined location
   * \return operation status
   */
  virtual ReturnStatus readData(void) = 0;

  /*!
   * Unlock device memory
   * \return operation status
   */
  virtual ReturnStatus unlockDevice(void) = 0;
  /*!
   * Lock device memory
   * \return operation status
   */
  virtual ReturnStatus lockDevice(void) = 0;

  /*!
   * Load and apply new settings
   */
  virtual void applySettings() = 0;

 protected:
  /*!
   * Check whether firmware file exists, is, in fact,
   * a file and has size equal to FIRMWARE_SIZE
   * \param[in] firmware firmware file to check
   * \return true if aforementioned conditions hild
   */
  bool checkFirmwareFile(const QFile* firmware);
  /*!
   * Check whether user data file exists, is, in fact,
   * a file and its size fits into flash user data section
   */
  bool checkDataFile(const QFile* data);

 private:
  //! \cond
  Q_DISABLE_COPY(IProgrammer);
  //! \endcond

 signals:
  /*! Emitted to log a new line
   */
  void logging(const QString& log);
};

#endif  // ABSTRACTPROGRAMMER_H
