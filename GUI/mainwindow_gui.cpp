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

  // Скрываем виджеты мастер доступа
  hideMasterAccessWidgets();
}

void MainWindow_GUI::hideMasterAccessWidgets() {
  MainPushButtonLayout->removeWidget(PB_AutoProgramDevice);
  MainPushButtonLayout->removeWidget(PB_EraseDevice);

  PB_EraseDevice->hide();
  PB_AutoProgramDevice->hide();

  MainLayout->removeWidget(GeneralLogGroup);
  GeneralLogGroup->hide();

  ServiceMenu->removeAction(CommonAccessRequestAct);
  ServiceMenu->addAction(MasterAccessRequestAct);
}

void MainWindow_GUI::showMasterAccessWidgets() {
  MainPushButtonLayout->addWidget(PB_AutoProgramDevice);
  MainPushButtonLayout->addWidget(PB_EraseDevice);

  PB_EraseDevice->show();
  PB_AutoProgramDevice->show();

  MainLayout->addWidget(GeneralLogGroup);
  GeneralLogGroup->show();

  ServiceMenu->removeAction(MasterAccessRequestAct);
  ServiceMenu->addAction(CommonAccessRequestAct);
}

void MainWindow_GUI::createActions() {
  ConnectAct = new QAction("Подключиться", MainWindow);
  ConnectAct->setObjectName(QString::fromUtf8("ConnectAct"));
  ConnectAct->setStatusTip("Подключиться к программатору");

  MasterAccessRequestAct = new QAction("Мастер доступ", MainWindow);
  MasterAccessRequestAct->setObjectName(
      QString::fromUtf8("MasterAccessRequestAct"));
  MasterAccessRequestAct->setStatusTip("Получить мастер доступ");

  CommonAccessRequestAct = new QAction("Обычный доступ", MainWindow);
  CommonAccessRequestAct->setObjectName(
      QString::fromUtf8("CommonAccessRequestAct"));
  CommonAccessRequestAct->setStatusTip("Получить обычный доступ");

  AboutProgramAct = new QAction("О программе", MainWindow);
  AboutProgramAct->setObjectName(QString::fromUtf8("AboutProgramAct"));
  AboutProgramAct->setStatusTip("Показать сведения о программе");
}

void MainWindow_GUI::createTopMenus() {
  ServiceMenu = MainWindow->menuBar()->addMenu("Сервис");
  ServiceMenu->addAction(ConnectAct);
  ServiceMenu->addSeparator();
  ServiceMenu->addAction(MasterAccessRequestAct);

  HelpMenu = MainWindow->menuBar()->addMenu("Справка");
  HelpMenu->addAction(AboutProgramAct);
  HelpMenu->addAction(AboutProgramAct);
}

void MainWindow_GUI::createWorkspaceWidgets()
{
  // Группа основных кнопок
  MainPushButtonGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(MainPushButtonGroup);

  MainPushButtonLayout = new QVBoxLayout();
  MainPushButtonGroup->setLayout(MainPushButtonLayout);

  PB_ManualProgramDevice = new QPushButton(QString("Загрузить прошивку"));
  PB_ManualProgramDevice->setObjectName(
      QString::fromUtf8("PB_ManualProgramDevice"));
  PB_ManualProgramDevice->setSizePolicy(QSizePolicy::Expanding,
                                        QSizePolicy::Expanding);
  PB_ManualProgramDevice->setFont(QFont("Arial", 16, QFont::Bold));
  MainPushButtonLayout->addWidget(PB_ManualProgramDevice);

  PB_AutoProgramDevice = new QPushButton(QString("Включить автозагрузку"));
  PB_AutoProgramDevice->setObjectName(
      QString::fromUtf8("PB_AutoProgramDevice"));
  PB_AutoProgramDevice->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Expanding);
  PB_AutoProgramDevice->setFont(QFont("Arial", 16, QFont::Bold));
  MainPushButtonLayout->addWidget(PB_AutoProgramDevice);

  PB_EraseDevice = new QPushButton(QString("Стереть прошивку"));
  PB_EraseDevice->setObjectName(QString::fromUtf8("PB_EraseDevice"));
  PB_EraseDevice->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  PB_EraseDevice->setFont(QFont("Arial", 16, QFont::Bold));
  MainPushButtonLayout->addWidget(PB_EraseDevice);

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
