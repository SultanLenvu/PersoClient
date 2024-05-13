#ifndef AbstractInputDialog_H
#define AbstractInputDialog_H

#include <QDialog>
#include <QtWidgets>

#include "types.h"

class AbstractInputDialog : public QDialog {
  Q_OBJECT

 public:
  explicit AbstractInputDialog(QWidget* parent = nullptr);
  virtual ~AbstractInputDialog() = default;

 public:
  virtual void getData(StringDictionary& data) const = 0;

 private:
  Q_DISABLE_COPY_MOVE(AbstractInputDialog);

 signals:
};

#endif  // AbstractInputDialog_H
