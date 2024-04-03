#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include <QRegularExpression>
#include <QSize>

#include "abstract_input_dialog.h"

class CustomTransponderStickerScanDialog : public AbstractInputDialog {
 private:
  QSize DesktopGeometry;

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

  QRegularExpression Regex;

 public:
  explicit CustomTransponderStickerScanDialog(QWidget* parent);
  ~CustomTransponderStickerScanDialog() = default;

  // AbstractInputDialog interface
 public:
  virtual void getData(StringDictionary& data) const override;

 private:
  Q_DISABLE_COPY_MOVE(CustomTransponderStickerScanDialog);

  void create(void);
  void createMatchTable(void);
  bool checkInput(void) const;
};

#endif  // CUSTOMTRANSPONDERSTICKERDIALOG_H
