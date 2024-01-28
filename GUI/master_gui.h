#ifndef MASTER_GUI_H
#define MASTER_GUI_H

#include "abstract_gui.h"

class MasterGui : public AbstractGui {
 public:
  QTabWidget* Tabs;

  // Виджеты для отображения логов
  QGroupBox* GeneralLogGroup;
  QVBoxLayout* GeneralLogLayout;
  QPlainTextEdit* GeneralLogs;

  /* Интерфейс для взаимодействия с сервером */
  //============================================================
  QWidget* ServerTab;
  QHBoxLayout* ServerTabMainLayout;

  QGroupBox* ServerControlPanel;
  QVBoxLayout* ServerControlPanelLayout;

  QPushButton* ServerConnectPushButton;
  QPushButton* ServerDisconnectButton;
  QPushButton* ServerEchoRequestButton;
  QPushButton* AuthorizePushButton;
  QSpacerItem* ServerControlPanelVS1;

  QPushButton* RequestBoxButton;
  QPushButton* GetCurrentBoxDataButton;
  QPushButton* RefundCurrentBoxButton;
  QPushButton* CompleteCurrentBoxButton;
  QSpacerItem* ServerControlPanelVS2;

  QPushButton* ReleaseTransponderButton;
  QPushButton* RereleaseTransponderButton;
  QPushButton* RollbackTransponderPushButton;
  QPushButton* GetCurrentTransponderDataButton;
  QPushButton* GetTransponderDataButton;
  QSpacerItem* ServerControlPanelVS3;

  QPushButton* PrintBoxStickerButton;
  QPushButton* PrintLastBoxStickerButton;
  QPushButton* PrintPalletStickerButton;
  QPushButton* PrintLastPalletStickerButton;

  QVBoxLayout* ModelViewLayout;
  QHBoxLayout* ModelViewSublayout;

  QGroupBox* ProductionLineDataGroup;
  QVBoxLayout* ProductionLineDataLayout;
  QTableView* ProductionLineDataView;

  QGroupBox* BoxDataGroup;
  QVBoxLayout* BoxDataLayout;
  QTableView* BoxDataView;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;
  //============================================================

  /* Интерфейс для взаимодействия с программатором */
  //============================================================
  QWidget* ProgrammatorTab;
  QHBoxLayout* ProgrammatorTabMainLayout;

  QGroupBox* ProgrammatorControlPanel;
  QVBoxLayout* ProgrammatorControlPanelLayout;

  QPushButton* ProgramDeviceButton;
  QPushButton* ReadDeviceFirmwareButton;
  QPushButton* EraseDeviceButton;
  QPushButton* ProgramDeviceUserDataButton;
  QPushButton* ReadDeviceUserDataButton;
  QSpacerItem* ButtonVS1;
  QPushButton* unlockMemoryButton;
  QPushButton* lockMemoryButton;
  //============================================================

  /* Интерфейс для взаимодействия с принтером */
  //============================================================
  QWidget* StickerPrinterTab;
  QHBoxLayout* StickerPrinterTabMainLayout;

  QGroupBox* StickerPrinterControlPanel;
  QVBoxLayout* StickerPrinterControlPanelLayout;

  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QSpacerItem* StickerPrinterControlPanelVS1;
  QPushButton* ExecuteStickerPrinterCommandScriptButton;

  QGroupBox* StickerPrinterCommandSriptGroup;
  QVBoxLayout* StickerPrinterCommandSriptLayout;
  QPlainTextEdit* StickerPrinterCommandSriptTextEdit;
  //============================================================

 public:
  explicit MasterGui(QWidget* parent);
  ~MasterGui();

  virtual void updateModelViews(void) override;
  virtual GuiType type(void) override;

 private:
  Q_DISABLE_COPY(MasterGui);
  void createServerTab(void);
  void createServerTabViews(void);

  void createProgrammatorTab(void);
  void createStickerPrinterTab(void);
  void createLogWidgets(void);

  void connectDepedencies(void);

 private slots:
  void displayLog(const QString& log);
  void clearLogDisplay(void);
};

#endif  // MASTER_GUI_H
