#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "General/definitions.h"

class CustomTransponderStickerScanDialog : public QDialog {
 private:
  QRect DesktopGeometry;

  QGridLayout* MainLayout;
  QLabel* SnLabel;
  QLineEdit* SnInput;
  QLabel* PanLabel;
  QLineEdit* PanInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  CustomTransponderStickerScanDialog(QWidget* parent);
  void getData(QHash<QString, QString>* data);

 private:
  Q_DISABLE_COPY(CustomTransponderStickerScanDialog);
  bool checkInput(void);
};

#endif // CUSTOMTRANSPONDERSTICKERDIALOG_H
