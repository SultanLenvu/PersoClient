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
  QSpacerItem* ServerControlPanelVS1;

  QPushButton* AuthorizePushButton;
  QPushButton* ReleaseTransponderButton;
  QPushButton* RereleaseTransponderButton;
  QPushButton* rollbackTransponderPushButton;

  QSpacerItem* ServerControlPanelVS2;
  QPushButton* PrintBoxStickerButton;
  QPushButton* PrintLastBoxStickerButton;
  QPushButton* PrintPalletStickerButton;
  QPushButton* PrintLastPalletStickerButton;

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
  QSpacerItem* ButtonVerticalSpacer;
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

  /* Настройки клиента */
  //============================================================
  QWidget* SettingsTab;
  QHBoxLayout* SettingsMainLayout;
  QVBoxLayout* SettingsMainSubLayout;
  QPushButton* ApplySettingsPushButton;

  // Настойки логгирования
  QGroupBox* LogSystemSettingsGroupBox;
  QGridLayout* LogSystemSettingsMainLayout;
  QLabel* LogSystemGlobalEnableLabel;
  QCheckBox* LogSystemGlobalEnableCheckBox;
  QLabel* LogSystemExtendedEnableLabel;
  QCheckBox* LogSystemExtendedEnableCheckBox;

  // Сеть
  QGroupBox* PersoSettingsGroupBox;
  QGridLayout* PersoSettingsMainLayout;
  QLabel* PersoServerIpAddressLabel;
  QLineEdit* PersoServerIpAddressLineEdit;
  QLabel* PersoServerPortLabel;
  QLineEdit* PersoServerPortLineEdit;

  // Программатор
  QGroupBox* ProgrammerSettingsGroupBox;
  QGridLayout* ProgrammerSettingsMainLayout;
  QLabel* ProgrammerExeFilePathLabel;
  QLineEdit* ProgrammerExeFilePathLineEdit;
  QPushButton* ProgrammerExeFilePathPushButton;
  QLabel* ProgrammerSpeedLabel;
  QLineEdit* ProgrammerSpeedLineEdit;

  // Принтер
  QGroupBox* StickerPrinterSettingsGroupBox;
  QGridLayout* StickerPrinterSettingsMainLayout;
  QLabel* StickerPrinterLibPathLabel;
  QLineEdit* StickerPrinterLibPathLineEdit;
  QPushButton* StickerPrinterLibPathPushButton;

  QSpacerItem* SettingsVS1;
  QSpacerItem* SettingsHS1;

 public:
  explicit MasterGui(QWidget* parent);
  ~MasterGui();

  virtual void update(void) override;

 public slots:
  void displayLogData(const QString& log);
  void clearLogDataDisplay(void);

 private:
  Q_DISABLE_COPY(MasterGui);
  void createServerTab(void);
  void createProgrammatorTab(void);
  void createSettingsTab(void);
  void createLogWidgets(void);

 private slots:
  void on_ProgrammerExeFilePathPushButton_slot(void);
  void on_StickerPrinterLibPathPushButton_slot(void);
};

#endif  // MASTER_GUI_H
