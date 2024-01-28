#ifndef PRODUCTION_ASSEMBLER_GUI_H
#define PRODUCTION_ASSEMBLER_GUI_H

#include <QTableView>

#include "abstract_gui.h"

class ProductionAssemblerGui : public AbstractGui {
 public:
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
  QPushButton* PrintCustomTransponderStickerButton;
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
  explicit ProductionAssemblerGui(QWidget* parent);
  ~ProductionAssemblerGui();

  virtual void updateModelViews(void) override;
  virtual GuiType type(void) override;

 private:
  Q_DISABLE_COPY_MOVE(ProductionAssemblerGui);

  void createBoxGroup(void);
  void createTransponderGroup(void);
  void createPrinterStickerGroup(void);
  void createModelViews(void);
};

#endif  // PRODUCTION_ASSEMBLER_GUI_H
