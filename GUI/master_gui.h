#ifndef MASTERGUI_H
#define MASTERGUI_H

#include "General/definitions.h"
#include "gui.h"

class MasterGUI : public GUI {
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

  QPushButton* PersoServerConnectPushButton;
  QPushButton* PersoServerDisconnectButton;
  QPushButton* PersoServerEchoRequestButton;
  QSpacerItem* ButtonVerticalSpacer1;

  QPushButton* MasterAuthorizePushButton;
  QPushButton* LoadTransponderFirmwareButton;
  QPushButton* ReloadTransponderFirmwareButton;

  QGroupBox* TransponderInfoGroup;
  QVBoxLayout* TransponderInfoLayout;
  QTableView* TransponderInfoView;
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
  QPushButton* UnlockDeviceButton;
  QPushButton* LockDeviceButton;
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
  QLabel* LogSystemSavePathLabel;
  QLineEdit* LogSystemSavePathLineEdit;
  QPushButton* LogSystemSavePathPushButton;

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
  //============================================================

 public:
  MasterGUI(QWidget* parent);

  virtual void create(void) override;
  virtual void update(void) override;

 public slots:
  void displayLogData(const QString& log);
  void clearLogDataDisplay(void);

 private:
  void createServerTab(void);
  void createProgrammatorTab(void);
  void createStickerPrinterTab(void);
  void createSettingsTab(void);
  void createLogWidgets(void);

 private slots:
  void on_LogSystemSavePathPushButton_slot(void);
  void on_ProgrammerExeFilePathPushButton_slot(void);
  void on_StickerPrinterLibPathPushButton_slot(void);
};

#endif  // MASTERGUI_H
