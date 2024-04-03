#ifndef TESTER_UNIT_USER_INTERFACE_H
#define TESTER_UNIT_USER_INTERFACE_H

#include <QtWidgets>

class TesterUnitUserInterface final : public QWidget {
 private:
  QHBoxLayout* MainLayout;

 private:
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderGroupLayout;
  QPushButton* RereleaseTransponderButton;

  QSpacerItem* ControlPanelVS;

  QGroupBox* PrinterStickerGroup;
  QVBoxLayout* PrinterStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintTransponderStickerButton;
  QSpacerItem* ControlPanelVS1;
  QPushButton* PrintLastBoxStickerButton;
  QPushButton* PrintBoxStickerButton;
  QSpacerItem* ControlPanelVS2;
  QPushButton* PrintLastPalletStickerButton;
  QPushButton* PrintPalletStickerButton;

 private:
  QVBoxLayout* ModelViewLayout;
  QHBoxLayout* ModelViewSublayout;

  QGroupBox* ProductionLineDataGroup;
  QVBoxLayout* ProductionLineDataLayout;
  QTableView* ProductionLineDataView;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

 public:
  explicit TesterUnitUserInterface(QWidget* parent = nullptr);
  ~TesterUnitUserInterface() = default;

 private:
  Q_DISABLE_COPY_MOVE(TesterUnitUserInterface);
  void connectDependencies(void);

  void create(void);
  void createTransponderGroup(void);
  void createPrinterStickerGroup(void);
  void createModelViews(void);
};

#endif  // TESTER_UNIT_USER_INTERFACE_H
