#ifndef GUI_KERNEL_H
#define GUI_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "General/hash_model.h"
#include "Log/log_system.h"
#include "abstract_gui.h"
#include "abstract_manager.h"
#include "interaction_system.h"

class GuiKernel : public QMainWindow {
  Q_OBJECT

 private:
  QSize DesktopGeometry;
  std::unique_ptr<AbstractGui> CurrentGUI;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QMenu* HelpMenu;

  QAction* OpenMasterInterfaceAct;
  QAction* OpenAuthorizationInterfaceAct;
  QAction* AboutProgramAct;
  //==================================================

  LogSystem* Logger;
  std::unique_ptr<QThread> LoggerThread;

  std::unordered_map<QString, std::shared_ptr<AbstractManager>> Managers;
  std::unique_ptr<QThread> ManagersThread;

  std::unique_ptr<InteractionSystem> Interactor;

  HashModel* TransponderDataModel;

 public:
  explicit GuiKernel(QWidget* parent = nullptr);
  ~GuiKernel();

 public slots:
  void on_AuthorizePushButton_slot(void);

  // Программатор
  void on_ProgramDeviceButton_slot(void);
  void on_ReadDeviceFirmwareButton_slot(void);
  void on_EraseDeviceButton_slot(void);
  void on_ProgramDeviceUserDataButton_slot(void);
  void on_ReadDeviceUserDataButton_slot(void);
  void on_UnlockDeviceButton_slot(void);
  void on_LockDeviceButton_slot(void);

  // Стикер принтер
  void on_PrintLastTransponderStickerButton_slot(void);
  void on_PrintCustomTransponderStickerButton_slot(void);
  void on_ExecuteStickerPrinterCommandScriptButton_slot(void);

  // Настройки
  void on_ApplySettingsPushButton_slot(void);

  // Верхнее меню
  void on_MasterInterfaceRequest_slot(void);
  void on_ProductionInterfaceRequest_slot(void);
  void on_AuthorizationInterfaceRequest_slot(void);

 private:
  Q_DISABLE_COPY_MOVE(GuiKernel);
  void loadSettings(void);
  bool checkNewSettings(void);

  void createAuthorizationInterface(void);
  void connectAuthorizationInterface(void);

  void createMasterInterface(void);
  void connectMasterInterface(void);

  void createProductionInterface(void);
  void connectProductionInterface(void);

  void createTestingInterface(void);
  void connectTestingInterface(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createManagerInstance(void);
  void createLoggerInstance(void);
  void createInteractorInstance(void);

  void createModels(void);
  void registerMetaTypes(void);

 private slots:
  void on_RequestProductionInterface_slot(void);
  void on_VisibilityChanged_slot(void);
  void displayTransponderData_slot(
      QSharedPointer<QHash<QString, QString>> data);

 signals:
  void applySettings_signal(void);

  // Сигналы для логгера
  void loggerClear_signal(void);

  void performLocalFirmwareLoading_signal(const QString& path);
  void performFirmwareReading_signal(void);
  void performFirmwareErasing_signal(void);
  void performDataReading_signal(void);
  void performDataLoading_signal(const QString& path);
  void performDeviceUnlock_signal(void);
  void performDeviceLock_signal(void);

  void performLastTransponderStickerPrinting_signal(void);
  void performCustomTransponderStickerPrinting_signal(
      const QSharedPointer<QHash<QString, QString>> data);
  void performStickerPrinterCommandScript_signal(
      const QSharedPointer<QStringList> commandScript);
};

#endif  // GUI_KERNEL_H
