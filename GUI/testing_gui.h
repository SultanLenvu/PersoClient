#ifndef TestingGUI_H
#define TestingGUI_H

#include "abstract_gui.h"

class TestingGUI : public AbstractGUI {
 public:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* ReloadTransponderFirmwareButton;

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
  explicit TestingGUI(QWidget* parent);
  ~TestingGUI();

  virtual void update(void) override;

 private:
  Q_DISABLE_COPY(TestingGUI);
};

#endif  // TestingGUI_H
