#ifndef PRODUCTIONMANAGERGUISUBKERNEL_H
#define PRODUCTIONMANAGERGUISUBKERNEL_H

#include <QStringListModel>

#include "abstract_gui_subkernel.h"
#include "hash_table_model.h"
#include "types.h"

class ProductionManagerGuiSubkernel final : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  HashTableModel ProductionLineModel;
  HashTableModel BoxModel;
  HashTableModel TransponderModel;

 public:
  explicit ProductionManagerGuiSubkernel();
  ~ProductionManagerGuiSubkernel() = default;
  Q_DISABLE_COPY_MOVE(ProductionManagerGuiSubkernel)

 public:
  HashTableModel& productionLineModel(void);
  HashTableModel& boxModel(void);
  HashTableModel& transponderModel(void);

 public slots:
  void logOn(const StringDictionary& param);
  void logOn(void);
  void logOut(void);

  void requestBox(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void rereleaseTransponder(void);
  void rollbackTransponder(void);

 public slots:
  void displayStateData(const StringDictionary& data);
  void displayBoxData(const StringDictionary& data);
  void displayTransponderData(const StringDictionary& data);

 private:
  void createModels(void);

 signals:
  void logOn_signal(const StringDictionary& param);
  void logOut_signal(void);

  void requestBox_signal(void);
  void refundCurrentBox_signal(void);
  void completeCurrentBox_signal(void);

  void releaseTransponder_signal(void);
  void rereleaseTransponder_signal(
      const StringDictionary& param);
  void rollbackTransponder_signal(void);
};

#endif  // PRODUCTIONMANAGERGUISUBKERNEL_H
