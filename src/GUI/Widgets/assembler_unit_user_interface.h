#ifndef ASSEMBLER_UNIT_USER_INTERFACE_H
#define ASSEMBLER_UNIT_USER_INTERFACE_H

#include <QTableView>
#include <QtWidgets>

class AssemblerUnitUserInterface final : public QWidget {
 private:
  QHBoxLayout* MainLayout;

  QVBoxLayout* ControlPanelLayout;

  QGroupBox* BoxGroup;
  QVBoxLayout* BoxGroupLayout;
  QPushButton* RequestBoxButton;
  QPushButton* RefundCurrentBoxButton;
  QPushButton* CompleteCurrentBoxButton;
  QSpacerItem* ButtonVS1;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderGroupLayout;
  QPushButton* ReleaseTransponderButton;
  QSpacerItem* ButtonVS2;
  QPushButton* RollbackTransponderPushButton;
  QPushButton* RereleaseTransponderButton;
  QSpacerItem* ButtonVS3;

  QGroupBox* PrinterStickerGroup;
  QVBoxLayout* PrinterStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;

  // Представления для моделей
  QVBoxLayout* ModelViewLayout;
  QHBoxLayout* ModelViewSublayout;

  QGroupBox* ProductionLineDataGroup;
  QVBoxLayout* ProductionLineDataLayout;
  QTableView* ProductionLineDataView;

  QGroupBox* BoxDataGroup;
  QVBoxLayout* BoxDataLayout;
  QTableView* BoxDataView;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit AssemblerUnitUserInterface(QWidget* parent = nullptr);
  ~AssemblerUnitUserInterface() = default;
  Q_DISABLE_COPY_MOVE(AssemblerUnitUserInterface);

 public:
  void setStateModel(QAbstractItemModel* model);
  void setTransponderModel(QAbstractItemModel* model);
  void setBoxModel(QAbstractItemModel* model);

 private:
  void createBoxGroup(void);
  void createTransponderGroup(void);
  void createPrinterStickerGroup(void);
  void createModelViews(void);

  void connectInternals(void);

 signals:
  void requestBox_trigger();
  void refundCurrentBox_trigger();
  void completeCurrentBox_trigger();

  void releaseTransponder_trigger();
  void rereleaseTransponder_trigger();
  void rollbackTransponder_trigger();

  void printLastTransponderSticker_trigger();
  void printTransponderSticker_trigger();
  void printBoxSticker_trigger();
};

#endif  // ASSEMBLER_UNIT_USER_INTERFACE_H
