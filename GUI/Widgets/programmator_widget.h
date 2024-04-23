#ifndef PROGRAMMATORUSERINTERFACE_H
#define PROGRAMMATORUSERINTERFACE_H

#include <QtWidgets>

class ProgrammatorWidget : public QWidget {
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
  explicit ProgrammatorWidget(QWidget* parent = nullptr);
  ~ProgrammatorWidget() = default;

 private:
  Q_DISABLE_COPY_MOVE(ProgrammatorWidget)
  void create(void);
  void connectDependencies(void);

 signals:
};

#endif  // PROGRAMMATORUSERINTERFACE_H
