#ifndef ISTICKERPRINTER_H
#define ISTICKERPRINTER_H

#include <QLibrary>
#include <QMap>
#include <QObject>
#include <QSettings>
#include <QtPrintSupport/QPrinterInfo>

class IStickerPrinter : public QObject {
  Q_OBJECT
 public:
  enum PrinterType {
    Unknown,
    TE310,
  };

  enum ReturnStatus {
    Failed,
    DllMissing,
    PrinterNotConnected,
    Completed,
  };

 protected:
  PrinterType Type;

 public:
  IStickerPrinter(QObject* parent, PrinterType type);

  virtual void printTransponderSticker(
      const QMap<QString, QString>* parameters) = 0;
  virtual void applySetting(void) = 0;

 signals:
  void logging(const QString& log);
  void operationFinished(ReturnStatus ret);
};

#endif  // ISTICKERPRINTER_H
