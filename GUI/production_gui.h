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
  QPushButton* PrintLastTransponderStickerButton;

  QGroupBox* TransponderInfoGroup;
  QVBoxLayout* TransponderInfoLayout;
  QTableView* TransponderInfoView;

 public:
  ProductionGUI(QWidget* parent);

  virtual void create(void) override;
  virtual void update(void) override;
};

#endif  // PRODUCTION_GUI_H
