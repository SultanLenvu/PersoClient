#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>

class TransponderStickerScanDialog : public QDialog {
  Q_OBJECT

 private:
  QRect DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* StickerDataInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  TransponderStickerScanDialog(QWidget* parent);
  void getData(QStringList* data);

 private:
  Q_DISABLE_COPY(TransponderStickerScanDialog);
};

#endif  // TRANSPONDERSTICKERDIALOG_H
