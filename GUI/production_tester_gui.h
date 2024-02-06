#ifndef PRODUCTION_TESTER_GUI_H
#define PRODUCTION_TESTER_GUI_H

#include "abstract_gui.h"

class ProductionTesterGui : public AbstractGui {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderGroupLayout;
  QPushButton* RereleaseTransponderButton;

  QSpacerItem* ControlPanelVS;

  QGroupBox* PrinterStickerGroup;
  QVBoxLayout* PrinterStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QSpacerItem* ControlPanelVS1;
  QPushButton* PrintLastBoxStickerButton;
  QPushButton* PrintBoxStickerButton;
  QSpacerItem* ControlPanelVS2;
  QPushButton* PrintLastPalletStickerButton;
  QPushButton* PrintPalletStickerButton;

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

  void create(void);
  void createTransponderGroup(void);
  void createPrinterStickerGroup(void);
  void createModelViews(void);
};

#endif  // PRODUCTION_TESTER_GUI_H
