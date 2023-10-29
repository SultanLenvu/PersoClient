#ifndef TestingGUI_H
#define TestingGUI_H

#include "abstract_gui.h"

class TestingGUI : public AbstractGUI {
 public:
  QGroupBox* ControlPanelGroup;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* ReloadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit TestingGUI(QWidget* parent);
  ~TestingGUI();

  virtual void update(void) override;

 private:
  Q_DISABLE_COPY(TestingGUI);
};

#endif  // TestingGUI_H
