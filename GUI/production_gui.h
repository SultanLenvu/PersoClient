#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QTableView>

#include "gui.h"

class ProductionGUI : public GUI {
 public:
  QGroupBox* ControlPanelGroup;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* LoadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* ReloadTransponderFirmwareButton;

  QGroupBox* TransponderSeedGroup;
  QVBoxLayout* TransponderSeedLayout;
  QTableView* TransponderSeedView;

 public:
  ProductionGUI(QWidget* parent);

  virtual void create(void) override;
  virtual void update(void) override;
};

#endif  // PRODUCTION_GUI_H
