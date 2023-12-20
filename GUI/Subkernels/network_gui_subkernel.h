#ifndef NETWORKGUISUBKERNEL_H
#define NETWORKGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "hash_model.h"
#include "types.h"

class NetworkGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  std::shared_ptr<AbstractGui> CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

  std::unique_ptr<HashModel> TransponderDataModel;

 public:
  explicit NetworkGuiSubkernel(const QString& name);
  ~NetworkGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void setCurrentGui(std::shared_ptr<AbstractGui> gui) override;

  void displayTransponderData(const std::shared_ptr<StringDictionary> data);

 private:
  Q_DISABLE_COPY_MOVE(NetworkGuiSubkernel);

  void connectAuthorizationGui(void);
  void connectProductionGui(void);
  void connectTestingGui(void);
  void connectMasterGui(void);

  void connect_guiSlot(void);
  void disconnect_guiSlot(void);
  void echoRequest_guiSlot(void);
  void authorize_guiSlot(void);
  void releaseTransponder_guiSlot(void);
  void rereleaseTransponder_guiSlot(void);
  void rollbackProductionLine_guiSlot(void);

  void printBoxSticker_guiSlot(void);
  void printLastBoxSticker_guiSlot(void);
  void printPalletSticker_guiSlot(void);
  void printLastPalletSticker_guiSlot(void);

 signals:
  void displayTestingGui(void);

  // Сигналы для менеджера
  void connectToServer_signal(void);
  void disconnectFromServer_signal(void);

  void echoServer_signal(void);
  void authorize_signal(const std::shared_ptr<StringDictionary> param);

  void releaseTransponder_signal();
  void rereleaseTransponder_signal(
      const std::shared_ptr<StringDictionary> param);
  void rollbackProductionLine_signal(void);

  void printBoxSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastBoxSticker_signal(void);
  void printPalletSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastPalletSticker_signal(void);
};

#endif  // NETWORKGUISUBKERNEL_H
