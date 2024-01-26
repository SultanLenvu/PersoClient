#ifndef ProductionGuiSubkernel_H
#define ProductionGuiSubkernel_H

#include "abstract_gui_subkernel.h"
#include "hash_table_model.h"

class ProductionGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 private:
  enum UserRole {
    Tester,
    Assembler,
  };

 private:
  UserRole Role;
  AbstractGui* CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

  std::unique_ptr<HashTableModel> BoxDataModel;
  std::unique_ptr<HashTableModel> TransponderDataModel;

 public:
  explicit ProductionGuiSubkernel(const QString& name);
  ~ProductionGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectGui(AbstractGui* gui) override;
  virtual void connectManager(
      std::shared_ptr<AbstractManager> manager) override;
  virtual void reset() override;

 public slots:
  void authorizationCompleted_slot(void);
  void displayTransponderData(const StringDictionary& data);
  void displayBoxData(const StringDictionary& data);

 private:
  Q_DISABLE_COPY_MOVE(ProductionGuiSubkernel);

  void connectAuthorizationGui(void);
  void connectMasterGui(void);
  void connectProductionAssemblerGui(void);
  void connectProductionTesterGui(void);

  void connectProductionManager(void) const;

  void connect_guiSlot(void);
  void disconnect_guiSlot(void);
  void echoRequest_guiSlot(void);
  void authorize_guiSlot(void);

  void requestBox_guiSlot(void);
  void getCurrentBoxData_guiSlot(void);
  void refundCurrentBox_guiSlot(void);
  void completeCurrentBox_guiSlot(void);

  void releaseTransponder_guiSlot(void);
  void rereleaseTransponder_guiSlot(void);
  void rollbackTransponder_guiSlot(void);
  void getCurrentTransponderData_guiSlot(void);
  void getTransponderData_guiSlot(void);

  void printBoxSticker_guiSlot(void);
  void printLastBoxSticker_guiSlot(void);
  void printPalletSticker_guiSlot(void);
  void printLastPalletSticker_guiSlot(void);

  void createModels(void);

 signals:
  void displayProductionAssemblerGui(void);
  void displayProductionTesterGui(void);
  void updateModelViews(void);

  // Сигналы для менеджера
  void connectToServer_signal(void);
  void disconnectFromServer_signal(void);
  void echoServer_signal(void);
  void authorize_signal(const std::shared_ptr<StringDictionary> param);

  void requestBox_signal(void);
  void getCurrentBoxData_signal(void);
  void refundCurrentBox_signal(void);
  void completeCurrentBox_signal(void);

  void releaseTransponder_signal();
  void rereleaseTransponder_signal(
      const std::shared_ptr<StringDictionary> param);
  void rollbackTransponder_signal(void);
  void getCurrentTransponderData_signal(void);
  void getTransponderData_signal(const std::shared_ptr<StringDictionary> param);

  void printBoxSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastBoxSticker_signal(void);
  void printPalletSticker_signal(const std::shared_ptr<StringDictionary> data);
  void printLastPalletSticker_signal(void);
};

#endif  // ProductionGuiSubkernel_H
