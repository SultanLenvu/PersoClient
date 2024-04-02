#ifndef MASTER_USER_INTERFACE_H
#define MASTER_USER_INTERFACE_H

#include <QtWidgets>

class MasterUserInterface final : public QWidget {
 public:
  QHBoxLayout* MainLayout;
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
  QPushButton* GetProductionLineDataButton;
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

  QWidget* ProgrammatorTab;
  QWidget* StickerPrinterTab;

 public:
  explicit MasterUserInterface(QWidget* parent = nullptr);
  ~MasterUserInterface() = default;

 private:
  Q_DISABLE_COPY_MOVE(MasterUserInterface);
  void create(void);
  void createServerTab(void);
  void createServerTabViews(void);

  void createProgrammatorTab(void);
  void createStickerPrinterTab(void);
  void createLogWidgets(void);

  void connectDepedencies(void);

 private slots:
  void displayLog(const QString& log);
  void clearLogDisplay(void);

 signals:
  void execCommandScript_signal(const QStringList& script);
};

#endif  // MASTER_USER_INTERFACE_H
