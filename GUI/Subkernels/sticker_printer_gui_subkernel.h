#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "types.h"

#include "sticker_printer_manager.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  std::shared_ptr<AbstractGui> CurrentGui;

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectAuthorizationGui(
      std::shared_ptr<AuthorizationGui> gui) override;
  virtual void connectMasterGui(std::shared_ptr<MasterGui> gui) override;
  virtual void connectProductionAssemblerGui(
      std::shared_ptr<ProductionAssemblerGui> gui) override;
  virtual void connectProductionTesterGui(std::shared_ptr<ProductionTesterGui> gui) override;
  virtual void resetCurrentGui() override;

 public:
  void connectManager(const StickerPrinterManager* manager) const;

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);

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
