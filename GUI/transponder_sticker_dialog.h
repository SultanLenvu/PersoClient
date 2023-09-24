#ifndef TransponderStickerDialog_H
#define TransponderStickerDialog_H

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

#endif  // TransponderStickerDialog_H
