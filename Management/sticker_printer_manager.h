#ifndef STICKERPRINTERMANAGER_H
#define STICKERPRINTERMANAGER_H

#include "abstract_manager.h"
#include "abstract_sticker_printer.h"

class StickerPrinterManager : public AbstractManager {
  Q_OBJECT
 private:
  std::unique_ptr<AbstractStickerPrinter> StickerPrinter;

 public:
  explicit StickerPrinterManager(const QString& name);
  ~StickerPrinterManager();

 public slots:
  virtual void applySettings(void) override;

  void printTransponderSticker(std::shared_ptr<StringDictionary> param);
  void printLastTransponderSticker(void);
  void printCustomTransponderSticker(std::shared_ptr<StringDictionary> param);
  void executeCommandScript(std::shared_ptr<QStringList> script);

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterManager)
  void sendLog(const QString& log);
  void loadSettings(void);

  void createStickerPrinter(void);

 signals:
};

#endif  // STICKERPRINTERMANAGER_H
