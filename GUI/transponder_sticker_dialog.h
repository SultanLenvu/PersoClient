#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>

class TransponderStickerDialog : public QDialog {
  Q_OBJECT

 private:
  QRect DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* PersoInitDataInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  TransponderStickerDialog(QWidget* parent);
  QString getData(void);
};

#endif  // TRANSPONDERSTICKERDIALOG_H
