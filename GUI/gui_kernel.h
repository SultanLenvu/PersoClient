#ifndef GUI_KERNEL_H
#define GUI_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "abstract_gui.h"
#include "abstract_gui_subkernel.h"
#include "abstract_manager.h"
#include "interaction_system.h"
#include "log_system.h"

class GuiKernel : public QMainWindow {
  Q_OBJECT

 private:
  QSize DesktopGeometry;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QAction* OpenMasterGuiAct;
  QAction* OpenAuthorizationGuiAct;
  QAction* OpenSettingsDialogAct;

  QMenu* HelpMenu;
  QAction* AboutProgramAct;
  //==================================================

  AbstractGui* CurrentGui;
  std::unordered_map<QString, std::shared_ptr<AbstractGuiSubkernel>> Subkernels;

  std::unique_ptr<LogSystem> Logger;
  std::unique_ptr<InteractionSystem> Interactor;
  std::unique_ptr<QThread> ServiceThread;

  std::unordered_map<QString, std::shared_ptr<AbstractManager>> Managers;
  std::unique_ptr<QThread> ManagersThread;

 public:
  explicit GuiKernel(QWidget* parent = nullptr);
  ~GuiKernel();

 public slots:
  void applySettings(void);

  void displayMasterGui_slot(void);
  void displayProductionAssemblerGui_slot(void);
  void displayProductionTesterGui_slot(void);
  void displayAuthorizationGui_slot(void);
  void displaySettingsDialog_slot(void);

 private:
  Q_DISABLE_COPY_MOVE(GuiKernel);
  void sendLog(const QString& log);
  void loadSettings(void);
  void registerMetaTypes(void);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createLoggerInstance(void);
  void createInteractorInstance(void);

  void createManagersInstance(void);

  void createGuiSubkernels(void);
  void createProductionGuiSubkernel(void);
  void createProgrammerGuiSubkernel(void);
  void createStickerPrinterGuiSubkernel(void);

  void createAuthorizationGui(void);
  void createMasterGui(void);
  void createProductionAssemblerGui(void);
  void createProductionTesterGui(void);
  void connectGui(void);

 signals:
  void applySettings_signal(void);

  void logging(const QString& log);
  void clearLogDisplay_signal(void);
};

#endif  // GUI_KERNEL_H
