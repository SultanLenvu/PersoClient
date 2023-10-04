#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "General/definitions.h"

class CustomTransponderStickerScanDialog : public QDialog {
 private:
  QRect DesktopGeometry;

  QGridLayout* MainLayout;
  QLabel* IssuerLabel;
  QLineEdit* IssuerInput;
  QLabel* ManufacturerIdLabel;
  QLineEdit* ManufacturerIdInput;
  QLabel* BatteryInsertationDateLabel;
  QLineEdit* BatteryInsertationDateInput;
  QLabel* SnLabel;
  QLineEdit* SnInput;
  QLabel* PanLabel;
  QLineEdit* PanInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  CustomTransponderStickerScanDialog(QWidget* parent);
  void getData(QMap<QString, QString>* data);

 private:
  bool checkInput(void);
};

#endif // CUSTOMTRANSPONDERSTICKERDIALOG_H
