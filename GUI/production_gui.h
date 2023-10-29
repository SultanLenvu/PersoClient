#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QTableView>

#include "abstract_gui.h"

class ProductionGUI : public AbstractGUI {
 public:
  QGroupBox* ControlPanelGroup;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* LoadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* RollbackProductionLinePushButton;
  QPushButton* ReloadTransponderFirmwareButton;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;

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
