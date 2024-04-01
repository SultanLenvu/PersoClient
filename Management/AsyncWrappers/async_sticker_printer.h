#ifndef STICKERPRINTERMANAGER_H
#define STICKERPRINTERMANAGER_H

#include "i_sticker_printer.h"
#include "progressable_async_wrapper.h"

class AsyncStickerPrinter final : public ProgressableAsyncWrapper {
  Q_OBJECT
 private:
  std::shared_ptr<IStickerPrinter> StickerPrinter;

 public:
  Q_INVOKABLE explicit AsyncStickerPrinter(
      const QString& name,
      std::shared_ptr<IStickerPrinter> isp);
  ~AsyncStickerPrinter() = default;

 public slots:
  void printTransponderSticker(const StringDictionary& param);
  void printLastTransponderSticker(void);

  void printBoxSticker(const StringDictionary& param);
  void printLastBoxSticker(void);

  void printPalletSticker(const StringDictionary& param);
  void printLastPalletSticker(void);

  void exec(const QStringList& script);

 private:
  Q_DISABLE_COPY_MOVE(AsyncStickerPrinter)
};

#endif // STICKERPRINTERMANAGER_H
