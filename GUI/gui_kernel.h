#ifndef GUI_KERNEL_H
#define GUI_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "Log/log_system.h"
#include "abstract_gui.h"
#include "abstract_gui_subkernel.h"
#include "abstract_manager.h"
#include "interaction_system.h"

class GuiKernel : public QMainWindow {
  Q_OBJECT

 private:
  QSize DesktopGeometry;
  std::shared_ptr<AbstractGui> CurrentGui;

  std::vector<std::shared_ptr<AbstractGuiSubkernel>> Subkernels;

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

 public:
  explicit GuiKernel(QWidget* parent = nullptr);
  ~GuiKernel();

 public slots:
  // Настройки
  void applySettingsPushButton_slot(void);

  // Верхнее меню
  void displayMasterInterface_slot(void);
  void displayProductionInterface_slot(void);
  void displayAuthorizationInterface_slot(void);

 private:
  Q_DISABLE_COPY_MOVE(GuiKernel);
  void loadSettings(void);
  bool checkNewSettings(void);

  void createAuthorizationInterface(void);
  void createMasterInterface(void);
  void createProductionInterface(void);
  void createTestingInterface(void);

  void connectCurrentGui(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createManagersInstance(void);
  void createLoggerInstance(void);
  void createInteractorInstance(void);

  void registerMetaTypes(void);

 private slots:
  void currentGuiVisibilityChanged_slot(void);
  void displayTransponderData_slot(
      QSharedPointer<QHash<QString, QString>> data);

 signals:
  void applySettings_signal(void);

  // Сигналы для логгера
  void loggerClear_signal(void);
};

#endif  // GUI_KERNEL_H
