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

#include "production_manager_gui_subkernel.h"
#include "programmer_gui_subkernel.h"
#include "server_connection_gui_subkernel.h"
#include "sticker_printer_gui_subkernel.h"

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

  std::unique_ptr<ProductionManagerGuiSubkernel> PMGS;
  std::unique_ptr<ProgrammerGuiSubkernel> PGS;
  std::unique_ptr<StickerPrinterGuiSubkernel> SPGS;
  std::unique_ptr<ServerConnectionGuiSubkernel> SCGS;

  std::unique_ptr<ProgressIndicator> PIndicator;
  std::unique_ptr<StatusIndicator> SIndicator;

  std::unique_ptr<AsyncEnvironment> Service;
  std::unique_ptr<AsyncEnvironment> Business;

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
