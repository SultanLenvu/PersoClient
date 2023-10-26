#ifndef MASTERGUI_H
#define MASTERGUI_H

#include "General/definitions.h"
#include "gui.h"

/*!
 * Master GUI mode
 */
class MasterGUI : public GUI {
 public:
  //! \cond
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
  //! \endcond

 public:
  /*!
   * Default constructor (no-op)
   * \param[in] parent QObject parent
   */
  MasterGUI(QWidget* parent);

  /*!
   * Create widgets and populate the window
   */
  virtual void create(void) override;
  /*!
   * Resize TransponderInfoView to match amount of columns
   */
  virtual void update(void) override;

 public slots:
  /*!
   * Show logs on the screen in GeneralLogs view
   * \param[in] log new log line
   */
  void displayLogData(const QString& log);
  /*!
   * Clear GeneralLogs view
   */
  void clearLogDataDisplay(void);

 private:
  //! \cond
  Q_DISABLE_COPY(MasterGUI);
  //! \endcond
  /*!
   * Create 'Server' tab widgets
   * \todo Qt Designer?
   */
  void createServerTab(void);
  /*!
   * Create 'Programmator' tab widgets
   */
  void createProgrammatorTab(void);
  /*!
   * Create 'Sticker printer' tab widgets
   */
  void createStickerPrinterTab(void);
  /*!
   * Create 'Settings' tab widgets
   */
  void createSettingsTab(void);
  /*!
   * Create logging-related widgets
   */
  void createLogWidgets(void);

 private slots:
  /*!
   * Show directory picker for log directory
   */
  void on_LogSystemSavePathPushButton_slot(void);
  /*!
   * Show file picker for JLink
   */
  void on_ProgrammerExeFilePathPushButton_slot(void);
  /*!
   * Show file picker for TSC library
   */
  void on_StickerPrinterLibPathPushButton_slot(void);
};

#endif  // MASTERGUI_H
