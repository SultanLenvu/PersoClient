#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  AbstractGui* CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

 public:
  explicit StickerPrinterGuiSubkernel(const QString& name);
  ~StickerPrinterGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectGui(AbstractGui* gui) override;
  virtual void connectManager(
      std::shared_ptr<AbstractManager> manager) override;
  virtual void reset() override;

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterGuiSubkernel);

  void connectAuthorizationGui();
  void connectMasterGui();
  void connectProductionAssemblerGui();
  void connectProductionTesterGui();

  void connectStickerPrinterManager(void) const;

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
