#ifndef STICKERPRINTERMANAGER_H
#define STICKERPRINTERMANAGER_H

#include "abstract_manager.h"

class StickerPrinterManager : public AbstractManager {
  Q_OBJECT
 public:
  explicit StickerPrinterManager(const QString& name);
  ~StickerPrinterManager();

 public slots:
  void printLastTransponderSticker(void);
  void printCustomTransponderSticker(std::shared_ptr<StringDictionary> param);
  void executeCommandScript(std::shared_ptr<QStringList> script);

 signals:
};

#endif  // STICKERPRINTERMANAGER_H
