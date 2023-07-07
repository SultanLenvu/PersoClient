#include "mainwindow_gui.h"

MainWindow_GUI::MainWindow_GUI(QMainWindow* mainWindow)
  : QObject(mainWindow)
{
  MainWindow = mainWindow;
}

void
MainWindow_GUI::create(void)
{
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
  MainWindow->setGeometry(
      DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1,
      DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5);
  MainWindow->setLayoutDirection(Qt::LeftToRight);

  MainWidget = new QWidget();
  MainWindow->setCentralWidget(MainWidget);

  MainLayout = new QHBoxLayout();
  MainWidget->setLayout(MainLayout);

  // Cоздаем обработчики пользовательских действий
  createActions();

  // Создаем верхнюю панель меню
  createTopMenus();

  // Создаем виджеты рабочего просмотранства
  createWorkspaceWidgets();

  // Подключаем неявное связывание сигналов и слотов по их имени
  QMetaObject::connectSlotsByName(MainWindow);
}

void MainWindow_GUI::createActions() {
  newAct = new QAction("New", MainWindow);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip("Create a new file");

  openAct = new QAction("Open...", MainWindow);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip("Open an existing file");

  saveAct = new QAction("Save", MainWindow);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip("Save the document to disk");

  printAct = new QAction("Print...", MainWindow);
  printAct->setShortcuts(QKeySequence::Print);
  printAct->setStatusTip("Print the document");

  exitAct = new QAction("Exit", MainWindow);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip("Exit the application");

  undoAct = new QAction("Undo", MainWindow);
  undoAct->setShortcuts(QKeySequence::Undo);
  undoAct->setStatusTip("Undo the last operation");

  redoAct = new QAction("Redo", MainWindow);
  redoAct->setShortcuts(QKeySequence::Redo);
  redoAct->setStatusTip("Redo the last operation");

  cutAct = new QAction("Cut", MainWindow);
  cutAct->setShortcuts(QKeySequence::Cut);
  cutAct->setStatusTip("Cut the current selection's contents to the "
                       "clipboard");

  copyAct = new QAction("Copy", MainWindow);
  copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip("Copy the current selection's contents to the "
                        "clipboard");

  pasteAct = new QAction("Paste", MainWindow);
  pasteAct->setShortcuts(QKeySequence::Paste);
  pasteAct->setStatusTip("Paste the clipboard's contents into the current "
                         "selection");

  boldAct = new QAction("Bold", MainWindow);
  boldAct->setCheckable(true);
  boldAct->setShortcut(QKeySequence::Bold);
  boldAct->setStatusTip("Make the text bold");

  QFont boldFont = boldAct->font();
  boldFont.setBold(true);
  boldAct->setFont(boldFont);

  italicAct = new QAction("Italic", MainWindow);
  italicAct->setCheckable(true);
  italicAct->setShortcut(QKeySequence::Italic);
  italicAct->setStatusTip("Make the text italic");

  QFont italicFont = italicAct->font();
  italicFont.setItalic(true);
  italicAct->setFont(italicFont);

  setLineSpacingAct = new QAction("Set &Line Spacing...", MainWindow);
  setLineSpacingAct->setStatusTip("Change the gap between the lines of a "
                                  "paragraph");

  setParagraphSpacingAct = new QAction("Set &Paragraph Spacing...", MainWindow);
  setParagraphSpacingAct->setStatusTip("Change the gap between paragraphs");

  aboutAct = new QAction("&About", MainWindow);
  aboutAct->setStatusTip("Show the application's About box");

  aboutQtAct = new QAction("About Qt", MainWindow);
  aboutQtAct->setStatusTip("Show the Qt library's About box");

  const QKeySequence ks;
  leftAlignAct = new QAction("Left Align", MainWindow);
  leftAlignAct->setCheckable(true);
  ks.mnemonic("Ctrl+L");
  leftAlignAct->setShortcut(ks);
  leftAlignAct->setStatusTip("Left align the selected text");

  rightAlignAct = new QAction("Right Align", MainWindow);
  rightAlignAct->setCheckable(true);
  ks.mnemonic("Ctrl+R");
  rightAlignAct->setShortcut(ks);
  rightAlignAct->setStatusTip("Right align the selected text");

  justifyAct = new QAction("Justify", MainWindow);
  justifyAct->setCheckable(true);
  ks.mnemonic("Ctrl+J");
  justifyAct->setShortcut(ks);
  justifyAct->setStatusTip("Justify the selected text");

  centerAct = new QAction("Center", MainWindow);
  centerAct->setCheckable(true);
  ks.mnemonic("Ctrl+E");
  centerAct->setShortcut(ks);
  centerAct->setStatusTip("Center the selected text");

  alignmentGroup = new QActionGroup(MainWindow);
  alignmentGroup->addAction(leftAlignAct);
  alignmentGroup->addAction(rightAlignAct);
  alignmentGroup->addAction(justifyAct);
  alignmentGroup->addAction(centerAct);
  leftAlignAct->setChecked(true);
}

void
MainWindow_GUI::createTopMenus()
{
  fileMenu = MainWindow->menuBar()->addMenu("File");
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(printAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = MainWindow->menuBar()->addMenu("Edit");
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);
  editMenu->addSeparator();

  helpMenu = MainWindow->menuBar()->addMenu("Help");
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);

  formatMenu = editMenu->addMenu("Format");
  formatMenu->addAction(boldAct);
  formatMenu->addAction(italicAct);
  formatMenu->addSeparator()->setText("Alignment");
  formatMenu->addAction(leftAlignAct);
  formatMenu->addAction(rightAlignAct);
  formatMenu->addAction(justifyAct);
  formatMenu->addAction(centerAct);
  formatMenu->addSeparator();
  formatMenu->addAction(setLineSpacingAct);
  formatMenu->addAction(setParagraphSpacingAct);
}

void MainWindow_GUI::createWorkspaceWidgets()
{
  // Группа основных кнопок
  MainPushButtonGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(MainPushButtonGroup);

  MainPushButtonLayout = new QVBoxLayout();
  MainPushButtonGroup->setLayout(MainPushButtonLayout);

  PB_EraseDevice = new QPushButton(QString("Стереть прошивку"));
  PB_EraseDevice->setObjectName(QString::fromUtf8("PB_EraseDevice"));
  PB_EraseDevice->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  MainPushButtonLayout->addWidget(PB_EraseDevice);

  PB_ManualProgramDevice = new QPushButton(QString("Загрузить прошивку"));
  PB_ManualProgramDevice->setObjectName(
      QString::fromUtf8("PB_ManualProgramDevice"));
  PB_ManualProgramDevice->setSizePolicy(QSizePolicy::Expanding,
                                        QSizePolicy::Expanding);
  MainPushButtonLayout->addWidget(PB_ManualProgramDevice);

  PB_AutoProgramDevice = new QPushButton(QString("Включить автозагрузку"));
  PB_AutoProgramDevice->setObjectName(
      QString::fromUtf8("PB_AutoProgramDevice"));
  PB_AutoProgramDevice->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Expanding);
  MainPushButtonLayout->addWidget(PB_AutoProgramDevice);

  VerticalSpacer1 =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
  MainPushButtonLayout->addItem(VerticalSpacer1);

  // Виджеты для отображения логов
  GeneralLogGroup = new QGroupBox("Логи");
  MainLayout->addWidget(GeneralLogGroup);

  GeneralLogLayout = new QVBoxLayout();
  GeneralLogGroup->setLayout(GeneralLogLayout);

  GeneralLogs = new QPlainTextEdit();
  GeneralLogs->setObjectName(QString::fromUtf8("GeneralLogs"));
  GeneralLogs->setEnabled(true);
  GeneralLogs->setTabletTracking(true);
  GeneralLogs->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  GeneralLogs->setCenterOnScroll(false);
  GeneralLogLayout->addWidget(GeneralLogs);
}
