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

void MainWindow_GUI::hideSystemWidgets() {}

void MainWindow_GUI::showSystemWidgets() {}

void MainWindow_GUI::createActions() {
  connectAct = new QAction("Подключиться", MainWindow);
  connectAct->setShortcuts(QKeySequence::New);
  connectAct->setStatusTip("Подключиться к программатору");

  systemAccessAct = new QAction("Мастер доступ", MainWindow);
  systemAccessAct->setShortcuts(QKeySequence::Open);
  systemAccessAct->setStatusTip("Мастер доступ");

  aboutProgramAct = new QAction("О программе", MainWindow);
  aboutProgramAct->setShortcuts(QKeySequence::Paste);
  aboutProgramAct->setStatusTip("Показать сведения о программе");
}

void MainWindow_GUI::createTopMenus() {
  fileMenu = MainWindow->menuBar()->addMenu("Сервис");
  fileMenu->addAction(connectAct);
  fileMenu->addSeparator();
  fileMenu->addAction(systemAccessAct);

  helpMenu = MainWindow->menuBar()->addMenu("Справка");
  helpMenu->addAction(aboutProgramAct);
  helpMenu->addAction(aboutProgramAct);
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
