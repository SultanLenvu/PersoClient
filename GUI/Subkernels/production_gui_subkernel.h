#ifndef ProductionGuiSubkernel_H
#define ProductionGuiSubkernel_H

#include "abstract_gui_subkernel.h"
#include "hash_table_model.h"
#include "production_assembler_gui.h"

class ProductionGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
  friend ProductionAssemblerGui;

 private:
  HashTableModel ProductionLineModel;
  HashTableModel BoxDataModel;
  HashTableModel TransponderDataModel;

 public:
  explicit ProductionGuiSubkernel(const QString& name);
  ~ProductionGuiSubkernel() = default;

 public slots:
  void displayProductionLineData(const StringDictionary& data);
  void displayTransponderData(const StringDictionary& data);
  void displayBoxData(const StringDictionary& data);

 private:
  Q_DISABLE_COPY_MOVE(ProductionGuiSubkernel);
  void connectDependecies(void);

 private slots:  // Слоты для сигналов от виджетов
  void connect_guiSlot(void);
  void disconnect_guiSlot(void);
  void echoRequest_guiSlot(void);
  void logOn_guiSlot(void);
  void getProductionLineData_guiSlot(void);

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

 signals:  // Сигналы для менеджера
  void connectToServer_signal(void);
  void disconnectFromServer_signal(void);
  void echoServer_signal(void);
  void launchProductionLine_signal(
      const std::shared_ptr<StringDictionary> param);
  void logOnServer_signal(const std::shared_ptr<StringDictionary> param);
  void getProductionLineData_signal(void);

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
