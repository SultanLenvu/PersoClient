#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QSize>

#include "General/definitions.h"
#include "input_dialog.h"

/*!
 * Transponder sticker dialog
 */
class TransponderStickerScanDialog : public InputDialog {
  Q_OBJECT

 private:
  //! \cond
  QSize DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* StickerData;
  QString pan;

  QHBoxLayout* ButtonLayout;
  QPushButton* AcceptButton;
  QPushButton* RejectButton;

  QSpacerItem* MainLayoutVS;
  //! \endcond

 public:
  /*!
   * Construct the dialog and create all the widgets and layouts
   * \param parent dialog parent
   */
  explicit TransponderStickerScanDialog(QWidget* parent);
  /*!
   * Default destructor
   */
  ~TransponderStickerScanDialog();

  /*!
   * Validate and put sticker parameters into data
   * \param[out] data Where to store parameters
   */
  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  //! \cond
  Q_DISABLE_COPY(TransponderStickerScanDialog);
  //! \endcond
  void create(void);
  /*!
   * Validate input. Check that:
   * - PAN is valid (\sa checkPan)
   * - SN is valid if it is entered (\sa checkSn)
   * \param[in] input Input to validate
   * \return true if PAN and SN are valid, false otherwise
   */
  bool checkInput(QStringList& input) const;
  /*!
   * Check that entered PAN is of length PAN_CHAR_LENGTH and consists
   * of digits only
   * \param[in] pan PAN to check
   * \return true if PAN is valid, false otherwise
   */
  bool checkPan(const QString& pan) const;
  /*!
   * Check that entered PAN is of length SERIAL_NUMBER_DS_LENGTH
   * and consists of digits only
   * \param[in] pan PAN to check
   * \return true if PAN is valid, false otherwise
   */
  bool checkSn(const QString& sn) const;
};

#endif  // TRANSPONDERSTICKERDIALOG_H
