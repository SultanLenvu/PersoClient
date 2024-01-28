#ifndef PRODUCTION_TESTER_GUI_H
#define PRODUCTION_TESTER_GUI_H

#include "abstract_gui.h"

class ProductionTesterGui : public AbstractGui {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderGroupLayout;
  QPushButton* RereleaseTransponderButton;

  QGroupBox* PrinterStickerGroup;
  QVBoxLayout* PrinterStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;
  QPushButton* PrintPalletStickerButton;

  QSpacerItem* ControlPanelVS;

  QVBoxLayout* ModelViewLayout;
  QHBoxLayout* ModelViewSublayout;

  QGroupBox* ProductionLineDataGroup;
  QVBoxLayout* ProductionLineDataLayout;
  QTableView* ProductionLineDataView;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit ProductionTesterGui(QWidget* parent);
  ~ProductionTesterGui();

  virtual void updateModelViews(void) override;
  virtual GuiType type(void) override;

 private:
  Q_DISABLE_COPY_MOVE(ProductionTesterGui);

  void createModelViews(void);
};

#endif  // PRODUCTION_TESTER_GUI_H
