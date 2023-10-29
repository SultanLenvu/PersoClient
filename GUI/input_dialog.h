#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QHash>
#include <QtWidgets>

#include "General/definitions.h"

class InputDialog : public QDialog
{
  Q_OBJECT
 public:
  enum DialogType {
    MasterPassword,
    Authorization,
    TransponderStickerScan,
    CustomTransponderSticker,
    PanInput,
  };
  Q_ENUM(DialogType);

 protected:
  DialogType Type;

 public:
   explicit InputDialog(QWidget* parent, DialogType type);
   virtual ~InputDialog();

   virtual void getData(QHash<QString, QString>* data) const = 0;

   DialogType type(void) const;

 private:
   Q_DISABLE_COPY(InputDialog);
};

#endif  // INPUTDIALOG_H
