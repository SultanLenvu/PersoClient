#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QtWidgets>

#include "gui.h"

class ProductionGUI : public GUI {
 public:
  QGroupBox* MainButtonGroup;
  QVBoxLayout* MainButtonLayout;

  QPushButton* ProgramDeviceButton;

  QSpacerItem* ButtonVerticalSpacer;

 public:
  ProductionGUI(QWidget* parent);

  virtual void create(void) override;
  virtual void update(void) override;
};

#endif  // PRODUCTION_GUI_H
