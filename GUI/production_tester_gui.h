#ifndef PRODUCTION_TESTER_GUI_H
#define PRODUCTION_TESTER_GUI_H

#include "abstract_gui.h"

class ProductionTesterGui : public AbstractGui {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* RereleaseTransponderButton;

  QGroupBox* PrintStickerGroup;
  QVBoxLayout* PrintStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;
  QPushButton* PrintPalletStickerButton;

  QSpacerItem* ControlPanelVS;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit ProductionTesterGui(QWidget* parent);
  ~ProductionTesterGui();

  virtual void update(void) override;
  virtual GuiType type(void) override;

 private:
  Q_DISABLE_COPY_MOVE(ProductionTesterGui);
};

#endif  // PRODUCTION_TESTER_GUI_H
