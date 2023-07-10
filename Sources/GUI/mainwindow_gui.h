#ifndef MAINWINDOW_GUI_H
#define MAINWINDOW_GUI_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>

class MainWindow_GUI : public QObject
{
  Q_OBJECT
public:
  QMainWindow* MainWindow;
  QRect DesktopGeometry;
  QWidget* MainWidget;
  QHBoxLayout* MainLayout;

  QVBoxLayout* ConnectButtonLayout;
  QPushButton* ConnectQFree;
  QPushButton* ConnectTopCon;
  QSpacerItem* ConnectButtonSpacer;


  // Виджеты главного окна
  //==================================================
  QGroupBox* MainPushButtonGroup;
  QVBoxLayout* MainPushButtonLayout;
  QPushButton *PB_EraseDevice;
  QPushButton *PB_ManualProgramDevice;
  QPushButton *PB_AutoProgramDevice;
  QSpacerItem *VerticalSpacer1;

  QGroupBox *GeneralLogGroup;
  QVBoxLayout *GeneralLogLayout;
  QPlainTextEdit* GeneralLogs;

  //==================================================

  // Виджеты верхней панели меню
  //==================================================
  QMenu *ServiceMenu;
  QMenu *HelpMenu;
  QAction *ConnectAct;
  QAction *MasterAccessRequestAct;
  QAction *CommonAccessRequestAct;
  QAction *AboutProgramAct;
  //==================================================

public:
  MainWindow_GUI(QMainWindow* mainWindow);

  void create(void);
  void hideMasterAccessWidgets(void);
  void showMasterAccessWidgets(void);

private:
  void createActions(void);
  void createTopMenus(void);
  void createWorkspaceWidgets(void);

signals:
};

#endif // MAINWINDOW_GUI_H
