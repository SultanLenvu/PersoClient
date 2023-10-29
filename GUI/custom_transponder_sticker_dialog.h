#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include "input_dialog.h"

class CustomTransponderStickerScanDialog : public InputDialog {
 private:
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

  QHash<QString, QString> MatchTable;

 public:
  explicit CustomTransponderStickerScanDialog(QWidget* parent);
  ~CustomTransponderStickerScanDialog();

  virtual void getData(QHash<QString, QString>* data) const override;

 private:
  Q_DISABLE_COPY(CustomTransponderStickerScanDialog);
  void create(void);
  void createMatchTable(void);
  bool checkInput(void) const;
};

#endif // CUSTOMTRANSPONDERSTICKERDIALOG_H
