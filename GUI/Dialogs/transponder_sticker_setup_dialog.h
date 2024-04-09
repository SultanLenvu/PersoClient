#ifndef CUSTOMTRANSPONDERSTICKERDIALOG_H
#define CUSTOMTRANSPONDERSTICKERDIALOG_H

#include <QRegularExpression>
#include <QSize>

#include "abstract_input_dialog.h"

class TransponderStickerSetupDialog : public AbstractInputDialog {
 private:
  QVBoxLayout* MainLayout;

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
  explicit TransponderStickerSetupDialog(QWidget* parent = nullptr);
  ~TransponderStickerSetupDialog() = default;

 public slots:  // QDialog interface
  virtual void accept(void) override;

 public:  // AbstractInputDialog interface
  virtual void getData(StringDictionary& data) const override;

 private:
  Q_DISABLE_COPY_MOVE(TransponderStickerSetupDialog);

  void create(void);
  bool checkInput(void) const;
};

#endif  // CUSTOMTRANSPONDERSTICKERDIALOG_H
