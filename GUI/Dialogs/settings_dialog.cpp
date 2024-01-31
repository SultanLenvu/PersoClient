#include "settings_dialog.h"
#include "General/definitions.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
  setObjectName("SettingsDialog");

  // Считываем размеры дисплея
  DesktopGeometry = QApplication::primaryScreen()->size();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.45, DesktopGeometry.height() * 0.45,
              DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1);
  setWindowTitle("Настройки");

  create();

  //  adjustSize();
  //  setFixedHeight(size().height());
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::create() {
  // Главный макет меню настроек
  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  // Система логгирования
  LogSystemGroupBox = new QGroupBox(QString("Система логгирования"));
  MainLayout->addWidget(LogSystemGroupBox);

  LogSystemMainLayout = new QGridLayout();
  LogSystemGroupBox->setLayout(LogSystemMainLayout);

  LogSystemGlobalEnableLabel = new QLabel("Глобальное логгирование вкл/выкл");
  LogSystemMainLayout->addWidget(LogSystemGlobalEnableLabel, 0, 0, 1, 1);
  LogSystemGlobalEnableCheckBox = new QCheckBox();
  LogSystemGlobalEnableCheckBox->setCheckState(
      Settings.value("log_system/global_enable").toBool() ? Qt::Checked
                                                          : Qt::Unchecked);
  LogSystemMainLayout->addWidget(LogSystemGlobalEnableCheckBox, 0, 1, 1, 1);
  LogSystemExtendedEnableLabel =
      new QLabel("Расширенное логгирование вкл/выкл");
  LogSystemMainLayout->addWidget(LogSystemExtendedEnableLabel, 1, 0, 1, 1);
  LogSystemExtendedEnableCheckBox = new QCheckBox();
  LogSystemExtendedEnableCheckBox->setCheckState(
      Settings.value("log_system/extended_enable").toBool() ? Qt::Checked
                                                            : Qt::Unchecked);
  LogSystemMainLayout->addWidget(LogSystemExtendedEnableCheckBox, 1, 1, 1, 1);
  LogSystemMessageMaxSizeLabel = new QLabel("Максимальный размер сообщения");
  LogSystemMainLayout->addWidget(LogSystemMessageMaxSizeLabel, 2, 0, 1, 1);
  LogSystemMessageMaxSizeLineEdit =
      new QLineEdit(Settings.value("log_system/message_max_size").toString());
  LogSystemMainLayout->addWidget(LogSystemMessageMaxSizeLineEdit, 2, 1, 1, 1);

  // Сеть
  PersoServerGroupBox = new QGroupBox(QString("Сетевые настройки"));
  MainLayout->addWidget(PersoServerGroupBox);

  PersoServerMainLayout = new QGridLayout();
  PersoServerGroupBox->setLayout(PersoServerMainLayout);

  PersoServerIpAddressLabel =
      new QLabel("IP адрес или URL сервера персонализации");
  PersoServerMainLayout->addWidget(PersoServerIpAddressLabel, 0, 0, 1, 1);
  PersoServerIpAddressLineEdit =
      new QLineEdit(Settings.value("perso_server_connection/ip").toString());
  PersoServerMainLayout->addWidget(PersoServerIpAddressLineEdit, 0, 1, 1, 1);
  PersoServerPortLabel = new QLabel("Порт сервера персонализации");
  PersoServerMainLayout->addWidget(PersoServerPortLabel, 1, 0, 1, 1);
  PersoServerPortLineEdit =
      new QLineEdit(Settings.value("perso_server_connection/port").toString());
  PersoServerMainLayout->addWidget(PersoServerPortLineEdit, 1, 1, 1, 1);

  // Настройки программатора
  ProgrammerGroupBox = new QGroupBox(QString("Программатор"));
  MainLayout->addWidget(ProgrammerGroupBox);

  ProgrammerMainLayout = new QGridLayout();
  ProgrammerGroupBox->setLayout(ProgrammerMainLayout);

  ProgrammerExeFilePathLabel = new QLabel("Путь к JLink.exe");
  ProgrammerMainLayout->addWidget(ProgrammerExeFilePathLabel, 0, 0, 1, 1);
  ProgrammerExeFilePathLineEdit = new QLineEdit(
      Settings.value("jlink_exe_programmer/exe_file_path").toString());
  ProgrammerMainLayout->addWidget(ProgrammerExeFilePathLineEdit, 0, 1, 1, 1);
  ProgrammerExeFilePathPushButton = new QPushButton("Обзор");
  ProgrammerMainLayout->addWidget(ProgrammerExeFilePathPushButton, 0, 2, 1, 1);
  connect(ProgrammerExeFilePathPushButton, &QPushButton::clicked, this,
          &SettingsDialog::programmerExeFilePathPushButton_slot);
  ProgrammerSpeedLabel = new QLabel("Скорость работы (кГц)");
  ProgrammerMainLayout->addWidget(ProgrammerSpeedLabel, 1, 0, 1, 1);
  ProgrammerSpeedLineEdit =
      new QLineEdit(Settings.value("jlink_exe_programmer/speed").toString());
  ProgrammerMainLayout->addWidget(ProgrammerSpeedLineEdit, 1, 1, 1, 2);

  // Настройки принтера
  StickerPrinterGroupBox = new QGroupBox(QString("Стикер-принтер"));
  MainLayout->addWidget(StickerPrinterGroupBox);

  StickerPrinterMainLayout = new QGridLayout();
  StickerPrinterGroupBox->setLayout(StickerPrinterMainLayout);

  StickerPrinterLibPathLabel = new QLabel("Путь к библиотеке");
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathLabel, 0, 0, 1, 1);
  StickerPrinterLibPathLineEdit =
      new QLineEdit(Settings.value("te310_printer/library_path").toString());
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathLineEdit, 0, 1, 1,
                                      1);
  StickerPrinterLibPathPushButton = new QPushButton("Обзор");
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathPushButton, 0, 2, 1,
                                      1);
  connect(StickerPrinterLibPathPushButton, &QPushButton::clicked, this,
          &SettingsDialog::stickerPrinterLibPathPushButton_slot);

  // Сжимаем позиционирование
  VS1 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  MainLayout->addItem(VS1);

  // Кнопки
  ButtonLayout = new QHBoxLayout();
  MainLayout->addLayout(ButtonLayout);

  ApplyPushButton = new QPushButton("Применить");
  ButtonLayout->addWidget(ApplyPushButton);
  connect(ApplyPushButton, &QPushButton::clicked, this,
          &SettingsDialog::accept);

  RejectPushButton = new QPushButton("Закрыть");
  ButtonLayout->addWidget(RejectPushButton);
  connect(RejectPushButton, &QPushButton::clicked, this, &QDialog::reject);
}

bool SettingsDialog::check() const {
  QHostAddress IP = QHostAddress(PersoServerIpAddressLineEdit->text());
  uint32_t speed = ProgrammerSpeedLineEdit->text().toUInt();
  int32_t msgMaxSize = LogSystemMessageMaxSizeLineEdit->text().toInt();

  if (msgMaxSize == 0) {
    return false;
  }

  if (IP.isNull()) {
    return false;
  }

  int32_t port = PersoServerPortLineEdit->text().toInt();
  if ((port > IP_PORT_MAX_VALUE) || (port < IP_PORT_MIN_VALUE)) {
    return false;
  }

  QFileInfo info(ProgrammerExeFilePathLineEdit->text());
  if (!info.exists()) {
    return false;
  }

  if (speed == 0) {
    return false;
  }

  info.setFile(StickerPrinterLibPathLineEdit->text());
  if (!info.exists()) {
    return false;
  }

  return true;
}

void SettingsDialog::save() {
  Settings.setValue("log_system/global_enable",
                    LogSystemGlobalEnableCheckBox->isChecked());
  Settings.setValue("log_system/extended_enable",
                    LogSystemExtendedEnableCheckBox->isChecked());
  Settings.setValue("log_system/message_max_size",
                    LogSystemMessageMaxSizeLineEdit->text().toInt());

  Settings.setValue("perso_server_connection/ip",
                    PersoServerIpAddressLineEdit->text());
  Settings.setValue("perso_server_connection/port",
                    PersoServerPortLineEdit->text().toInt());

  Settings.setValue("jlink_exe_programmer/exe_file_path",
                    ProgrammerExeFilePathLineEdit->text());
  Settings.setValue("jlink_exe_programmer/speed",
                    ProgrammerSpeedLineEdit->text());

  Settings.setValue("te310_printer/library_path",
                    StickerPrinterLibPathLineEdit->text());
}

void SettingsDialog::accept() {
  if (!check()) {
    QMessageBox::critical(this, "Ошибка", "Некорректный ввод данных.",
                          QMessageBox::Ok);
    return;
  }

  save();
  QMessageBox::information(this, "Оповещение", "Новые настройки применены.",
                           QMessageBox::Ok);
  emit applyNewSettings();
}

void SettingsDialog::programmerExeFilePathPushButton_slot() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "");
  ProgrammerExeFilePathLineEdit->setText(filePath);
}

void SettingsDialog::stickerPrinterLibPathPushButton_slot() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Выберите файл", "");
  StickerPrinterLibPathLineEdit->setText(filePath);
}
