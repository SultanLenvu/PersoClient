#include "master_gui.h"

MasterGUI::MasterGUI(QObject *parent) : GUI(parent, Master) {}

QWidget *MasterGUI::create() {
  // Группа основных кнопок
  MainButtonGroup = new QGroupBox("Панель управления");
  MainLayout->addWidget(MainButtonGroup);

  MainButtonLayout = new QVBoxLayout();
  MainButtonGroup->setLayout(MainButtonLayout);

  ProgramDeviceButton = new QPushButton(QString("Загрузить прошивку"));
  ProgramDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
  ProgramDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(ProgramDeviceButton);

  EraseDeviceButton = new QPushButton(QString("Стереть прошивку"));
  EraseDeviceButton->setObjectName(QString::fromUtf8("PB_EraseDevice"));
  EraseDeviceButton->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
  EraseDeviceButton->setFont(QFont("Arial", 16, QFont::Bold));
  MainButtonLayout->addWidget(EraseDeviceButton);

  ButtonVerticalSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum);
  MainButtonLayout->addItem(ButtonVerticalSpacer);

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
