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
  Q_DISABLE_COPY_MOVE(ProgrammatorWidget)

 private:
  void create(void);
  void connectInternals(void);

 private slots:
  void programMemory();
  void programUserData();

 signals:
  void programMemory_trigger(const QString& path);
  void readMemory_trigger();
  void eraseMemory_trigger();

  void readUserData_trigger();
  void programUserData_trigger(const QString& path);
  void readUcid_trigger();

  void unlockMemory_trigger();
  void lockMemory_trigger();
};

#endif  // PROGRAMMATORUSERINTERFACE_H
