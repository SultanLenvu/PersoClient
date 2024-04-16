#ifndef MASTER_USER_INTERFACE_H
#define MASTER_USER_INTERFACE_H

#include <QtWidgets>

class MasterUserInterface final : public QWidget {
 private:
  QHBoxLayout* MainLayout;
  QTabWidget* Tabs;

  QWidget* LogDisplay;
  QWidget* ProductionManagerTab;
  QWidget* ProgrammatorTab;
  QWidget* StickerPrinterTab;

 public:
  explicit MasterUserInterface(QWidget* parent = nullptr);
  ~MasterUserInterface() = default;

 private:
  Q_DISABLE_COPY_MOVE(MasterUserInterface);
  void create(void);
  void createProductionManagerTab(void);
  void createProgrammatorTab(void);
  void createStickerPrinterTab(void);
  void createLogWidgets(void);
};

#endif  // MASTER_USER_INTERFACE_H
