#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "types.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel() = default;
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);

 public slots:
  void printTransponderSticker(void);
  void printLastTransponderSticker(void);
  void exec(const QStringList& script);

 signals:
  void printLastTransponderSticker_signal(void);
  void printTransponderSticker_signal(const StringDictionary& param);
  void exec_signal(const QStringList& script);
};

#endif  // STICKERPRINTERGUISUBKERNEL_H
