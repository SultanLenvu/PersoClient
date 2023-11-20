#ifndef TRANSPONDERSTICKERDIALOG_H
#define TRANSPONDERSTICKERDIALOG_H

#include <QSize>

#include "General/definitions.h"
#include "input_dialog.h"

class TransponderStickerScanDialog : public InputDialog {
  Q_OBJECT

 private:
  QSize DesktopGeometry;

  QVBoxLayout* MainLayout;
  QLabel* MainLabel;
  QPlainTextEdit* StickerData;
  QString pan;

  QHBoxLayout* ButtonLayout;
  QPushButton* AcceptButton;
  QPushButton* RejectButton;

  QSpacerItem* MainLayoutVS;

 public:
  explicit TransponderStickerScanDialog(QWidget* parent);
  ~TransponderStickerScanDialog();

  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  Q_DISABLE_COPY(TransponderStickerScanDialog);
  void create(void);
  bool checkInput(QStringList& input) const;
  bool checkPan(const QString& pan) const;
  bool checkSn(const QString& sn) const;
};

#endif  // TRANSPONDERSTICKERDIALOG_H
