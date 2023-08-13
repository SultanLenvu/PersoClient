#ifndef MASTERGUI_H
#define MASTERGUI_H

#include "gui.h"

class MasterGUI : public GUI
{
public:
  QGroupBox *MainButtonGroup;
  QVBoxLayout *MainButtonLayout;

  // Кнопки
  QPushButton *AutoProgramDeviceButton;
  QPushButton *ManualProgramDeviceButton;
  QPushButton *ReadDeviceFirmwareButton;
  QPushButton *EraseDeviceButton;

  QPushButton *ProgramDeviceUserDataButton;
  QPushButton *ReadDeviceUserDataButton;

  QPushButton *UnlockDeviceButton;
  QPushButton *LockDeviceButton;

  QPushButton* PersoServerConnectPushButton;
  QPushButton* PersoServerDisconnectButton;
  QPushButton* PersoServerSendEchoButton;

  QSpacerItem *ButtonVerticalSpacer;

  // Виджеты для отображения логов
  QGroupBox *GeneralLogGroup;
  QVBoxLayout *GeneralLogLayout;
  QPlainTextEdit *GeneralLogs;

public:
  MasterGUI(QObject *parent);

  virtual QWidget *create(void) override;
  virtual void update(void) override;

public slots:
  void displayLogData(const QString &log);
};

#endif // MASTERGUI_H
