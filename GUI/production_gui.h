#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QTableView>

#include "abstract_gui.h"

class ProductionGUI : public AbstractGUI {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* LoadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* RollbackProductionLinePushButton;
  QPushButton* ReloadTransponderFirmwareButton;

  QGroupBox* PrintStickerGroup;
  QVBoxLayout* PrintStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit ProductionGUI(QWidget* parent);
  ~ProductionGUI();

  virtual void update(void) override;

 private:
  Q_DISABLE_COPY(ProductionGUI);
};

#endif  // PRODUCTION_GUI_H
