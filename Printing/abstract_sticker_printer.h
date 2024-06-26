#ifndef ISTICKERPRINTER_H
#define ISTICKERPRINTER_H

#include <QHash>
#include <QLibrary>
#include <QObject>
#include <QSettings>
#include <QtPrintSupport/QPrinterInfo>

#include "types.h"

class AbstractStickerPrinter : public QObject {
  Q_OBJECT
 public:
  enum StickerPrinterType {
    Unknown,
    TE310,
  };
  Q_ENUM(StickerPrinterType);

 public:
  AbstractStickerPrinter(const QString& name);
  virtual ~AbstractStickerPrinter();

  virtual ReturnStatus checkConfig(void) = 0;
  virtual StickerPrinterType type(void) = 0;

  virtual ReturnStatus printTransponderSticker(
      const StringDictionary& param) = 0;
  virtual ReturnStatus printLastTransponderSticker(void) = 0;

  virtual ReturnStatus printBoxSticker(const StringDictionary& param) = 0;
  virtual ReturnStatus printLastBoxSticker(void) = 0;

  virtual ReturnStatus printPalletSticker(const StringDictionary& param) = 0;
  virtual ReturnStatus printLastPalletSticker(void) = 0;

  virtual ReturnStatus exec(const QStringList& commandScript) = 0;

  virtual void applySetting(void) = 0;

 private:
  Q_DISABLE_COPY_MOVE(AbstractStickerPrinter);

 signals:
  void logging(const QString& log);
};

#endif  // ISTICKERPRINTER_H
