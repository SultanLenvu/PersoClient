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
#include "asynchronous_object_space.h"
#include "progress_indicator.h"
#include "service_object_space.h"
#include "status_indicator.h"

class GuiKernel : public QMainWindow {
  Q_OBJECT

 private:
  QSize DesktopGeometry;

  // Верхнее меню
  //==================================================
  QMenu* ServiceMenu;
  QAction* OpenMasterGuiAct;
  QAction* LogOutServerAct;
  QAction* OpenSettingsDialogAct;

  QMenu* HelpMenu;
  QAction* AboutProgramAct;
  //==================================================

  AbstractGui* CurrentGui;
  std::vector<AbstractGuiSubkernel> Subkernels;

  std::unique_ptr<ProgressIndicator> PIndicator;
  std::unique_ptr<StatusIndicator> SIndicator;

  std::unique_ptr<ServiceObjectSpace> Service;
  std::unique_ptr<AsynchronousObjectSpace> Async;

 public:
  explicit GuiKernel(QWidget* parent = nullptr);
  ~GuiKernel() = default;

 public slots:
  void applySettings(void);

  void displayMasterGui_slot(void);
  void displayProductionAssemblerGui_slot(void);
  void displayProductionTesterGui_slot(void);
  void logOutServerAct_slot(void);
  void displaySettingsDialog_slot(void);

  void onServerDisconnected(void);

 private:
  Q_DISABLE_COPY_MOVE(GuiKernel);

  void createTopMenuActions(void);
  void createTopMenu(void);

  void createLoggerInstance(void);
  void createReactions(void);

  void createManagersInstance(void);

  void createGuiSubkernels(void);

  void createAuthorizationGui(void);
  void createMasterGui(void);
  void createProductionAssemblerGui(void);
  void createProductionTesterGui(void);
  void connectGui(void);

 signals:
  void logOutServer_signal(void);
};

#endif  // GUI_KERNEL_H
