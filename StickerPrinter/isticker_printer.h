#ifndef ISTICKERPRINTER_H
#define ISTICKERPRINTER_H

#include <QLibrary>
#include <QMap>
#include <QObject>
#include <QSettings>

class IStickerPrinter : public QObject {
  Q_OBJECT
 public:
  enum PrinterType {
    Unknown,
    TE310,
  };

 protected:
  PrinterType Type;

 public:
  IStickerPrinter(QObject* parent, PrinterType type);

  virtual void print(const QMap<QString, QString>* parameters) = 0;
  virtual void applySetting(void) = 0;

 signals:
  void logging(const QString& log);
};

#endif  // ISTICKERPRINTER_H
