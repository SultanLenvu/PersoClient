#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "types.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel() = default;

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);

  void connectStickerPrinterManager(void) const;

  void printLastTransponderSticker_guiSlot(void);
  void printCustomTransponderSticker_guiSlot(void);
  void executeStickerPrinterCommandScript_guiSlot(void);

 signals:
  void printLastTransponderSticker_signal(void);
  void printCustomTransponderSticker_signal(StringDictionary& param);
  void executeCommandScript_signal(std::shared_ptr<QStringList> script);
};

#endif  // STICKERPRINTERGUISUBKERNEL_H
