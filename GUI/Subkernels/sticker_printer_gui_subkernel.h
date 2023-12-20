#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "types.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  std::shared_ptr<AbstractGui> CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void setCurrentGui(std::shared_ptr<AbstractGui> gui) override;

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);

  void connectProductionGui(void);
  void connectTestingGui(void);
  void connectMasterGui(void);

  void printLastTransponderSticker_guiSlot(void);
  void printCustomTransponderSticker_guiSlot(void);
  void executeStickerPrinterCommandScript_guiSlot(void);

 signals:
  void printLastTransponderSticker_signal(void);
  void printCustomTransponderSticker_signal(
      std::shared_ptr<StringDictionary> param);
  void executeCommandScript_signal(std::shared_ptr<QStringList> script);
};

#endif  // STICKERPRINTERGUISUBKERNEL_H
