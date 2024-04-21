#ifndef GUI_KERNEL_H
#define GUI_KERNEL_H

#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QTextStream>
#include <QtWidgets>

#include "abstract_gui_subkernel.h"
#include "async_environment.h"
#include "progress_indicator.h"
#include "status_indicator.h"

class GuiKernel : public QMainWindow {
  Q_OBJECT
 public:
  enum GuiKernelMode {
    Authorization,
    AssemblyUnit,
    TesterUnit,
    Master,
  };
  Q_ENUM(GuiKernelMode)

 private:
  QSize DesktopGeometry;
  GuiKernelMode CurrentMode;

  // Верхнее меню
  //==================================================
  QWidget* ServiceMenu;
  QAction* OpenMasterInterfaceAct;
  QAction* LogOutServerAct;
  QAction* SettingsDialogAction;

  QWidget* HelpMenu;
  QAction* AboutProgramAct;
  //==================================================

  std::vector<std::unique_ptr<AsyncEnvironment>> Environments;
  QHash<QString, std::shared_ptr<AbstractGuiSubkernel>> Subkernels;

  std::unique_ptr<ProgressIndicator> PIndicator;
  std::unique_ptr<StatusIndicator> SIndicator;

 public:
  explicit GuiKernel(QWidget* parent = nullptr);
  ~GuiKernel() = default;
  Q_DISABLE_COPY_MOVE(GuiKernel);

 public slots:
  void displayMasterInterface(void);
  void displayAssemblerUnitInterface(void);
  void displayTesterUnitInterface(void);

  void logOutServerAct_slot(void);

 private:
  void createTopMenuActions(void);
  void createTopMenu(void);

  void createEnvironments(void);
  void createReactions(void);
  void createGuiSubkernels(void);

  void createAuthorizationUserInterface(void);
  void createMasterInterface(void);
  void createAssemblerUnitUserInterface(void);
  void createProductionTesterGui(void);

 private slots:
  void displaySettingsDialog(void);

 signals:
  void logOutServer_signal(void);
};

#endif  // GUI_KERNEL_H
