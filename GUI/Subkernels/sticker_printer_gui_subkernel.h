#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "types.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel() = default;

 public slots:
  void printTransponderSticker(void);
  void printLastTransponderSticker(void);
  void exec(const QStringList& script);

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);
  void connectDependecies(void);

 signals:
  void printLastTransponderSticker_signal(void);
  void printTransponderSticker_signal(const StringDictionary& param);
  void exec_signal(const QStringList& script);
};

#endif  // STICKERPRINTERGUISUBKERNEL_H
