#ifndef PERSOSERVERWIDGET_H
#define PERSOSERVERWIDGET_H

#include "abstract_user_interface.h"
#include "server_commands_widget.h"
#include "types.h"

class ProductionManagerWidget final : public AbstractUserInterface {
  Q_OBJECT

 private:
  QHBoxLayout* MainLayout;
  QVBoxLayout* SubLayout;

 private:
  ServerCommandsWidget* CommandsWidget;

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
  void setProductionLineModel(QAbstractItemModel* model);
  void setBoxModel(QAbstractItemModel* model);
  void setTransponderModel(QAbstractItemModel* model);
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
  void connectInternals(void);

 private slots:
  void logOn_clicked(void);
  void rereleaseTransponder_clicked(void);

 signals:
  void connect_trigger();
  void disconnect_trigger();
  void echo_trigger();

  void launchProductionLine_trigger(const StringDictionary& param);
  void shutdownProductionLine_trigger();
  void getProductionLineData_trigger();

  void requestBox_trigger();
  void getCurrentBoxData_trigger();
  void refundCurrentBox_trigger();
  void completeCurrentBox_trigger();

  void getTransponderFirmware_trigger();
  void confirmTransponderFirmware_trigger(const StringDictionary& param);
  void repeatTransponderFirmware_trigger(const StringDictionary& param);
  void confirmRepeatedTransponderFirmware_trigger(
      const StringDictionary& param);
  void rollbackTransponder_trigger();
  void getCurrentTransponderData_trigger();
  void getTransponderData_trigger(const StringDictionary& param);

  void printBoxSticker_trigger(const StringDictionary& data);
  void printLastBoxSticker_trigger();
  void printPalletSticker_trigger(const StringDictionary& data);
  void printLastPalletSticker_trigger();

  void logOn_trigger(const StringDictionary& param);
  void logOut_trigger();

  //  void requestBox_trigger();
  void refundBox_trigger();
  void completeBox_trigger();

  void releaseTransponder_trigger();
  void rereleaseTransponder_trigger(const StringDictionary& param);
  //  void rollbackTransponder_trigger();

  void programMemory_trigger(const QString& path);
  void readMemory_trigger();
  void eraseMemory_trigger();

  void readUserData_trigger();
  void programUserData_trigger(const QString& path);
  void readUcid_trigger();

  void unlockMemory_trigger();
  void lockMemory_trigger();
};

#endif  // PERSOSERVERWIDGET_H
