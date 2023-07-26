#ifndef MASTERGUI_H
#define MASTERGUI_H

#include "gui.h"

class MasterGUI : public GUI
{
public:
  QGroupBox *MainButtonGroup;
  QVBoxLayout *MainButtonLayout;

  // Кнопки
  QPushButton *MasterProgramDeviceButton;
  QPushButton *ProgramDeviceUserDataButton;
  QPushButton *EraseDeviceButton;

  QPushButton *UnlockDeviceButton;
  QPushButton *ReadDeviceFirmwareButton;
  QPushButton *ReadDeviceUserDataButton;

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
