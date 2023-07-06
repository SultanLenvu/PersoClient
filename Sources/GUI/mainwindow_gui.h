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
  QPushButton* PB_ManualProgramChip;
  QPushButton *PB_AutoProgramChip;
  QSpacerItem *VerticalSpacer1;

  QGroupBox *GeneralLogGroup;
  QVBoxLayout *GeneralLogLayout;
  QPlainTextEdit* GeneralLogs;

  //==================================================

  // Виджеты верхней панели меню
  //==================================================
  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* formatMenu;
  QMenu* helpMenu;
  QActionGroup* alignmentGroup;
  QAction* newAct;
  QAction* openAct;
  QAction* saveAct;
  QAction* printAct;
  QAction* exitAct;
  QAction* undoAct;
  QAction* redoAct;
  QAction* cutAct;
  QAction* copyAct;
  QAction* pasteAct;
  QAction* boldAct;
  QAction* italicAct;
  QAction* leftAlignAct;
  QAction* rightAlignAct;
  QAction* justifyAct;
  QAction* centerAct;
  QAction* setLineSpacingAct;
  QAction* setParagraphSpacingAct;
  QAction* aboutAct;
  QAction* aboutQtAct;
  QLabel* infoLabel;
  //==================================================

public:
  MainWindow_GUI(QMainWindow* mainWindow);

  void create(void);

private:
  void createActions(void);
  void createTopMenus(void);
  void createWorkspaceWidgets(void);

signals:
};

#endif // MAINWINDOW_GUI_H
