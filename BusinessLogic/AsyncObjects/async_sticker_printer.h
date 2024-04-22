#ifndef STICKERPRINTERMANAGER_H
#define STICKERPRINTERMANAGER_H

#include "i_sticker_printer.h"
#include "progressable_async_wrapper.h"

class AsyncStickerPrinter final : public AbstractAsyncObject {
  Q_OBJECT
 private:
  std::shared_ptr<IStickerPrinter> StickerPrinter;

 public:
  explicit AsyncStickerPrinter(const QString& name,
                               std::shared_ptr<IStickerPrinter> printer);
  ~AsyncStickerPrinter() = default;
  Q_DISABLE_COPY_MOVE(AsyncStickerPrinter)

 public slots:
  void printTransponderSticker(const StringDictionary& param);
  void printLastTransponderSticker(void);

  void printBoxSticker(const StringDictionary& param);
  void printLastBoxSticker(void);

  void printPalletSticker(const StringDictionary& param);
  void printLastPalletSticker(void);

  void exec(const QStringList& script);
};

#endif // STICKERPRINTERMANAGER_H
