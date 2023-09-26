#ifndef TE310PRINTER_H
#define TE310PRINTER_H

#include "isticker_printer.h"

class TE310Printer : public IStickerPrinter {
  Q_OBJECT

 private:
  QString DriverPath;
  QLibrary* Driver;

 public:
  explicit TE310Printer(QObject* parent);

  virtual void print(const QMap<QString, QString>* parameters) override;
  virtual void applySetting(void) override;

 private:
  void loadSetting();
};

#endif // TE310PRINTER_H
