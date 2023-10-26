#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>

/*!
 * Transponder sticker dialog
 */
class TransponderStickerScanDialog : public QDialog {
  Q_OBJECT

 private:
  //! \cond
  QRect DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* StickerDataInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;
  //! \endcond

 public:
  /*!
   * Construct the dialog and create all the widgets and layouts
   * \param parent dialog parent
   */
  TransponderStickerScanDialog(QWidget* parent);
  /*!
   * Put sticker parameters into data
   * \param[out] data Where to store parameters
   */
  void getData(QStringList* data);

 private:
  //! \cond
  Q_DISABLE_COPY(TransponderStickerScanDialog);
  //! \endcond
};

#endif  // TRANSPONDERSTICKERDIALOG_H
