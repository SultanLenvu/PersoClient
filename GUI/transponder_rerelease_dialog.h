#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QDialog>
#include <QtWidgets>

class TransponderRereleaseDialog : public QDialog {
  Q_OBJECT

 private:
  QRect DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* PersoInitDataInput;
  QPushButton* OkButton;
  QPushButton* CancelButton;

 public:
  TransponderRereleaseDialog(QWidget* parent);
  QString getData(void);
};

#endif  // TRANSPONDERSTICKERDIALOG_H
