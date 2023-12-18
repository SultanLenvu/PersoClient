#ifndef AbstractInputDialog_H
#define AbstractInputDialog_H

#include <QDialog>
#include <QtWidgets>

#include "types.h"

class AbstractInputDialog : public QDialog {
  Q_OBJECT
 public:
  enum InputDialogType {
    MasterPassword,
    Authorization,
    TransponderStickerScan,
    CustomTransponderSticker,
    PanInput,
  };
  Q_ENUM(InputDialogType);

 public:
  explicit AbstractInputDialog(QWidget* parent);
  virtual ~AbstractInputDialog();

  virtual void getData(StringDictionary& data) const = 0;
  virtual InputDialogType type(void) const = 0;

 private:
  Q_DISABLE_COPY_MOVE(AbstractInputDialog);

 signals:
};

#endif  // AbstractInputDialog_H
