#ifndef ProductionGuiSubkernel_H
#define ProductionGuiSubkernel_H

#include "abstract_gui_subkernel.h"
#include "hash_model.h"
#include "production_manager.h"
#include "types.h"

class ProductionGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 private:
  enum UserRole {
    Tester,
    Assembler,
  };

 private:
  std::shared_ptr<AbstractGui> CurrentGui;

  std::unique_ptr<HashModel> TransponderDataModel;

  UserRole CurrentRole;

 public:
  explicit ProductionGuiSubkernel(const QString& name);
  ~ProductionGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectAuthorizationGui(
      std::shared_ptr<AuthorizationGui> gui) override;
  virtual void connectMasterGui(std::shared_ptr<MasterGui> gui) override;
  virtual void connectProductionAssemblerGui(
      std::shared_ptr<ProductionAssemblerGui> gui) override;
  virtual void connectProductionTesterGui(
      std::shared_ptr<ProductionTesterGui> gui) override;
  virtual void resetCurrentGui() override;

 public:
  void connectManager(const ProductionManager* manager) const;

 public slots:
  void authorizationCompleted_slot(void);
  void displayTransponderData(const std::shared_ptr<StringDictionary> data);

 private:
  Q_DISABLE_COPY_MOVE(ProductionGuiSubkernel);

  void connect_guiSlot(void);
  void disconnect_guiSlot(void);
  void echoRequest_guiSlot(void);
  void authorize_guiSlot(void);
  void releaseTransponder_guiSlot(void);
  void rereleaseTransponder_guiSlot(void);
  void rollbackTransponder_guiSlot(void);

  void printBoxSticker_guiSlot(void);
  void printLastBoxSticker_guiSlot(void);
  void printPalletSticker_guiSlot(void);
  void printLastPalletSticker_guiSlot(void);

 signals:
  void displayProductionAssemblerGui(void);
  void displayProductionTesterGui(void);

  // Сигналы для менеджера
  void connectToServer_signal(void);
  void disconnectFromServer_signal(void);

  void echoServer_signal(void);
  void authorize_signal(const std::shared_ptr<StringDictionary> param);

  void releaseTransponder_signal();
  void rereleaseTransponder_signal(
      const std::shared_ptr<StringDictionary> param);
  void rollbackTransponder_signal(void);

  void printBoxSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastBoxSticker_signal(void);
  void printPalletSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastPalletSticker_signal(void);
};

#endif  // ProductionGuiSubkernel_H
