#ifndef PRODUCTION_ASSEMBLER_GUI_H
#define PRODUCTION_ASSEMBLER_GUI_H

#include <QTableView>

#include "abstract_gui.h"

class ProductionAssemblerGui : public AbstractGui {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* ReleaseTransponderButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* RollbackTransponderPushButton;
  QPushButton* RereleaseTransponderButton;

  QGroupBox* PrintStickerGroup;
  QVBoxLayout* PrintStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;

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
};

#endif  // PRODUCTION_ASSEMBLER_GUI_H
