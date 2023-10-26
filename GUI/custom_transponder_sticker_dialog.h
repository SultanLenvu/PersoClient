#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "General/definitions.h"

/*!
 * Dialog for creating custom stickers
 */
class CustomTransponderStickerScanDialog : public QDialog {
 private:
  //! \cond
  QRect DesktopGeometry;

  QGridLayout* MainLayout;
  QLabel* SnLabel;
  QLineEdit* SnInput;
  QLabel* PanLabel;
  QLineEdit* PanInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;
  //! \endcond

 public:
  /*!
   * Construct object and widget layout
   * \param[in] parent dialog parent
   */
  CustomTransponderStickerScanDialog(QWidget* parent);
  /*!
   * Validate user input and store it into provided map.
   * The following information will be stored:
   * - Issuer name
   * - PAN
   * - S/N
   * - Battery insertion date
   * - Manufacturer ID
   * \param[out] data map to store user input into
   */
  void getData(QMap<QString, QString>* data);

 private:
  //! \cond
  Q_DISABLE_COPY(CustomTransponderStickerScanDialog);
  //! \endcond
  /*! Validate input
   * Check that:
   * - Manufacturer ID has length of MANUFACTURER_ID_DS_CHAR_LENGTH
   *   and is not 0;
   * - Battery insertion has length of BATTERY_INSERTATION_DATE_DS_CHAR_LENGTH
   *   and is not 0;
   * - S/N has length of SERIAL_NUMBER_DS_CHAR_LENGTH and is not 0;
   * - PAN has length of PAN_CHAR_LENGTH;
   * - PAN has no non-digits
   * \return true if configuration is valid
   */
  bool checkInput(void);
};

#endif // CUSTOMTRANSPONDERSTICKERDIALOG_H
