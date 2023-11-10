#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include "input_dialog.h"

/*!
 * Dialog for creating custom stickers
 */
class CustomTransponderStickerScanDialog : public InputDialog {
 private:
  //! \cond
  QRect DesktopGeometry;

  QGridLayout* MainLayout;

  QHBoxLayout* StickerFormatLayout;
  QLabel* StickerFormatLabel;
  QComboBox* StickerFormatChoice;

  QHBoxLayout* SnLayout;
  QLabel* SnLabel;
  QLineEdit* SnInput;

  QHBoxLayout* PanLayout;
  QLabel* PanLabel;
  QLineEdit* PanInput;

  QHBoxLayout* ButtonLayout;
  QPushButton* AcceptButton;
  QPushButton* RejectButton;

  QSpacerItem* MainLayoutVS;
  //! \endcond

  QHash<QString, QString> MatchTable;

 public:
  /*!
   * Construct object and widget layout
   * \param[in] parent dialog parent
   */
  explicit CustomTransponderStickerScanDialog(QWidget* parent);
  /*!
   * Default destructor
   */
  ~CustomTransponderStickerScanDialog();

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
  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  //! \cond
  Q_DISABLE_COPY(CustomTransponderStickerScanDialog);
  //! \endcond
  /*!
   * Initialize layout
   */
  void create(void);
  /*!
   * sticker format -> vendor mapping
   */
  void createMatchTable(void);
  /*! Validate input
   * Check that:
   * - S/N has length of SERIAL_NUMBER_DS_CHAR_LENGTH
   * - PAN has length of PAN_CHAR_LENGTH;
   * - PAN and S/N have no non-digits
   * \return true if configuration is valid
   */
  bool checkInput(void) const;
};

#endif // CUSTOMTRANSPONDERSTICKERDIALOG_H
