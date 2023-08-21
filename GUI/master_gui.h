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

  /* Интерфейс для взаимодействия с программатором */
  //============================================================
  QWidget* ProgrammatorTab;
  QHBoxLayout* ProgrammatorTabMainLayout;

  QGroupBox* ProgrammatorControlPanel;
  QVBoxLayout* ProgrammatorControlPanelLayout;

  QPushButton* ServerProgramDeviceButton;
  QPushButton* ManualProgramDeviceButton;
  QPushButton* ReadDeviceFirmwareButton;
  QPushButton* EraseDeviceButton;

  QPushButton* ProgramDeviceUserDataButton;
  QPushButton* ReadDeviceUserDataButton;

  QPushButton* UnlockDeviceButton;
  QPushButton* LockDeviceButton;

  QSpacerItem* ButtonVerticalSpacer;
  //============================================================

  /* Интерфейс для взаимодействия с сервером */
  //============================================================
  QWidget* ServerTab;
  QHBoxLayout* ServerTabMainLayout;

  QGroupBox* ServerControlPanel;
  QVBoxLayout* ServerControlPanelLayout;

  QPushButton* PersoServerConnectPushButton;
  QPushButton* PersoServerDisconnectButton;
  QPushButton* PersoServerEchoRequestButton;
  QPushButton* PersoServerFirmwareRequestButton;

  QSpacerItem* ButtonVerticalSpacer1;
  //============================================================

  /* Настройки клиента */
  //============================================================
  QWidget* SettingsTab;
  QHBoxLayout* SettingsMainLayout;
  QVBoxLayout* SettingsMainSubLayout;
  QPushButton* ApplySettingsPushButton;

  // Персонализация
  QGroupBox* PersoSettingsGroupBox;
  QGridLayout* PersoSettingsMainLayout;
  QLabel* UsePersoServerLabel;
  QCheckBox* UsePersoServerCheckBox;
  QLabel* PersoServerIpAddressLabel;
  QLineEdit* PersoServerIpAddressLineEdit;
  QLabel* PersoServerPortLabel;
  QLineEdit* PersoServerPortLineEdit;

  QSpacerItem* SettingsVerticalSpacer1;
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
  void createSettingsTab(void);
  void createLogWidgets(void);
};

#endif  // MASTERGUI_H
