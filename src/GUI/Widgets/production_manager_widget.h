#ifndef PERSOSERVERWIDGET_H
#define PERSOSERVERWIDGET_H

#include "abstract_user_interface.h"
#include "types.h"

class ProductionManagerWidget final : public AbstractUserInterface {
  Q_OBJECT

 private:
  QHBoxLayout* MainLayout;
  QVBoxLayout* SubLayout;

 private:
  QWidget* CommandsWidget;

 private:
  QGroupBox* ControlPanel;
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* InitGroup;
  QVBoxLayout* InitGroupLayout;
  QPushButton* LogOnPushButton;
  QPushButton* LogOutPushButton;

  QGroupBox* BoxGroup;
  QVBoxLayout* BoxGroupLayout;
  QPushButton* RequestBoxPushButton;
  QPushButton* RefundBoxPushButton;
  QPushButton* CompleteCurrentBoxPushButton;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderGroupLayout;
  QPushButton* ReleaseTransponderPushButton;
  QPushButton* RereleaseTransponderPushButton;
  QPushButton* RollbackTransponderPushButton;

  QSpacerItem* ControlPanelVS;

 private:  // Отображение данных
  QGroupBox* DataDisplayPanel;
  QGridLayout* DataDisplayLayout;

  QGroupBox* ProductionLineDataGroup;
  QVBoxLayout* ProductionLineDataLayout;
  QTableView* ProductionLineDataView;

  QGroupBox* BoxDataGroup;
  QVBoxLayout* BoxDataLayout;
  QTableView* BoxDataView;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;

  QGroupBox* FirmwareGroup;
  QVBoxLayout* FirmwareLayout;
  QPlainTextEdit* FirmwareView;

 public:
  explicit ProductionManagerWidget(QWidget* parent = nullptr);
  ~ProductionManagerWidget() = default;
  Q_DISABLE_COPY_MOVE(ProductionManagerWidget)

 public:
  void displayFirmware(const QByteArray& firmware);

 private:
  void createWidgets(void);
  void createControlPanel(void);
  void createCommandsWidget(void);
  void createInitGroup(void);
  void createBoxGroup(void);
  void createTransponderGroup(void);
  void createPrintingGroup(void);
  void createDataDisplayPanel(void);

 signals:
  void logOn_trigger(const StringDictionary& param);
  void logOut_trigger();

  void requestBox_trigger();
  void releaseTransponder_trigger();
};

#endif  // PERSOSERVERWIDGET_H
