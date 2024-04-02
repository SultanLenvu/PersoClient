#ifndef ASSEMBLER_UNIT_USER_INTERFACE_H
#define ASSEMBLER_UNIT_USER_INTERFACE_H

#include <QTableView>
#include <QtWidgets>

class AssemblerUnitUserInterface final : public QWidget {
 public:
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

 private:
  Q_DISABLE_COPY_MOVE(AssemblerUnitUserInterface);
  void connectDependecies(void);

 private:
  void createBoxGroup(void);
  void createTransponderGroup(void);
  void createPrinterStickerGroup(void);
  void createModelViews(void);
};

#endif  // ASSEMBLER_UNIT_USER_INTERFACE_H
