#include "master_gui.h"

MasterGUI::MasterGUI(QObject *parent) : GUI(parent, Master) {}

QWidget *MasterGUI::create() {
  // Группа основных кнопок
  MainButtonGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(MainButtonGroup);

  MainButtonLayout = new QVBoxLayout();
  MainButtonGroup->setLayout(MainButtonLayout);

  AutoProgramDeviceButton =
      new QPushButton(QString("Автоматическая зарузка прошивки"));
  AutoProgramDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                         QSizePolicy::Expanding);
  AutoProgramDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(AutoProgramDeviceButton);

  ManualProgramDeviceButton =
      new QPushButton(QString("Ручная зарузка прошивки"));
  ManualProgramDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Expanding);
  ManualProgramDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ManualProgramDeviceButton);

  ReadDeviceFirmwareButton = new QPushButton(QString("Считать прошивку"));
  ReadDeviceFirmwareButton->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
  ReadDeviceFirmwareButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ReadDeviceFirmwareButton);

  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  EraseDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
  EraseDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(EraseDeviceButton);

  ProgramDeviceUserDataButton =
      new QPushButton(QString("Загрузить  пользовательские данные"));
  ProgramDeviceUserDataButton->setSizePolicy(QSizePolicy::Expanding,
                                             QSizePolicy::Expanding);
  ProgramDeviceUserDataButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ProgramDeviceUserDataButton);

  ReadDeviceUserDataButton =
      new QPushButton(QString("Считать пользовательские данные"));
  ReadDeviceUserDataButton->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
  ReadDeviceUserDataButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ReadDeviceUserDataButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
  MainButtonLayout->addItem(ButtonVerticalSpacer);

  UnlockDeviceButton = new QPushButton(QString("Разблокировать память"));
  UnlockDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
  UnlockDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(UnlockDeviceButton);

  LockDeviceButton = new QPushButton(QString("Заблокировать память"));
  LockDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);
  LockDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(LockDeviceButton);

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

  return MainWidget;
}

void MasterGUI::update() {}

void MasterGUI::displayLogData(const QString &log) {
  if (GeneralLogs->toPlainText().count() > 100000)
    GeneralLogs->clear();

  GeneralLogs->appendPlainText(log);
}
