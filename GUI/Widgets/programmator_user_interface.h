#ifndef PROGRAMMATORUSERINTERFACE_H
#define PROGRAMMATORUSERINTERFACE_H

#include <QtWidgets>

class ProgrammatorUserInterface : public QWidget {
  Q_OBJECT
 private:
  QHBoxLayout* MainLayout;

  QGroupBox* ControlPanel;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* ProgramDeviceButton;
  QPushButton* ReadDeviceFirmwareButton;
  QPushButton* EraseDeviceButton;
  QPushButton* ProgramDeviceUserDataButton;
  QPushButton* ReadDeviceUserDataButton;
  QPushButton* ReadDeviceUcidButton;
  QSpacerItem* ButtonVS1;
  QPushButton* UnlockMemoryButton;
  QPushButton* LockMemoryButton;

 public:
  explicit ProgrammatorUserInterface(QWidget* parent = nullptr);
  ~ProgrammatorUserInterface() = default;

 private:
  Q_DISABLE_COPY_MOVE(ProgrammatorUserInterface)
  void create(void);
  void connectDependencies(void);

 signals:
};

#endif  // PROGRAMMATORUSERINTERFACE_H
