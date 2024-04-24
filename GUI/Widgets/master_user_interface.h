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
  explicit MasterUserInterface(QWidget* pwg,
                               QWidget* pw,
                               QWidget* spw,
                               QWidget* ldw);
  ~MasterUserInterface() = default;
  Q_DISABLE_COPY_MOVE(MasterUserInterface);

  void addMenu();
  void setLogDiplay(QWidget* ld);

 private:
  void createTabs(QWidget* pwg, QWidget* pw, QWidget* spw);
};

#endif  // MASTER_USER_INTERFACE_H
