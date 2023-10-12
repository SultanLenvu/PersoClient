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
  enum StickerPrinterType {
    Unknown,
    TE310,
  };
  Q_ENUM(StickerPrinterType);

  enum ReturnStatus {
    ParameterError,
    LibraryMissing,
    NotConnected,
    Failed,
    Completed,
  };
  Q_ENUM(ReturnStatus);

 protected:
  StickerPrinterType Type;

 public:
  IStickerPrinter(QObject* parent, StickerPrinterType type);

  virtual ReturnStatus printTransponderSticker(
      const QMap<QString, QString>* parameters) = 0;
  virtual ReturnStatus printLastTransponderSticker(void) = 0;
  virtual ReturnStatus exec(const QStringList* commandScript) = 0;

  virtual void applySetting(void) = 0;

 signals:
  void logging(const QString& log);
  void operationFinished(ReturnStatus ret);
};

#endif  // ISTICKERPRINTER_H
