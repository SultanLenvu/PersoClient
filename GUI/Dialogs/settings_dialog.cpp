#include "settings_dialog.h"
#include "configuration_system.h"
#include "definitions.h"
#include "global_environment.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
  // Создаем виджеты
  create();
  adjustSize();

  // Задаем заголовок
  setWindowTitle("Настройки");
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

void SettingsDialog::create() {
  // Главный макет меню настроек
  MainLayout = new QVBoxLayout();
  MainLayout->setSizeConstraint(QLayout::SetFixedSize);
  MainLayout->setAlignment(Qt::AlignTop);
  setLayout(MainLayout);

  // Система логгирования
  createLogSystem();

  // Сеть
  createPersoServer();

  // Настройки программатора
  createProgrammator();

  // Настройки принтера
  createStickerPrinter();

  // Кнопки
  createButtons();

  // Сжимаем позиционирование
  //  VS1 = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
  //                        QSizePolicy::MinimumExpanding);
  //  MainLayout->addItem(VS1);
}

void SettingsDialog::connectDependencies() {
  ConfigurationSystem* cum =
      GlobalEnvironment::instance()->getObject<ConfigurationSystem>(
          "ConfigurationSystem");

  connect(this, &SettingsDialog::applyNewSettings, cum,
          &ConfigurationSystem::applySettings_signal);
}

void SettingsDialog::createLogSystem() {
  LogSystemGroupBox = new QGroupBox(QString("Система логгирования"));
  MainLayout->addWidget(LogSystemGroupBox);

  LogSystemMainLayout = new QGridLayout();
  LogSystemGroupBox->setLayout(LogSystemMainLayout);

  LogSystemGlobalEnableLabel = new QLabel("Активно");
  LogSystemMainLayout->addWidget(LogSystemGlobalEnableLabel, 0, 0, 1, 3);
  LogSystemGlobalEnableCheckBox = new QCheckBox();
  LogSystemGlobalEnableCheckBox->setCheckState(
      Settings.value("log_system/global_enable").toBool() ? Qt::Checked
                                                          : Qt::Unchecked);
  LogSystemMainLayout->addWidget(LogSystemGlobalEnableCheckBox, 0, 1, 1, 3);

  LogSystemMessageMaxSizeLabel = new QLabel("Максимальный размер сообщения");
  LogSystemMainLayout->addWidget(LogSystemMessageMaxSizeLabel, 1, 0, 1, 3);
  LogSystemMessageMaxSizeLineEdit =
      new QLineEdit(Settings.value("log_system/message_max_size").toString());
  LogSystemMainLayout->addWidget(LogSystemMessageMaxSizeLineEdit, 1, 1, 1, 3);

  LogSystemFileDirLabel = new QLabel("Директория для лог-файлов");
  LogSystemMainLayout->addWidget(LogSystemFileDirLabel, 2, 0, 1, 1);
  LogSystemFileDirLineEdit =
      new QLineEdit(Settings.value("log_system/file_directory").toString());
  LogSystemMainLayout->addWidget(LogSystemFileDirLineEdit, 2, 1, 1, 1);
  LogSystemFileDirPushButton = new QPushButton("Обзор");
  LogSystemMainLayout->addWidget(LogSystemFileDirPushButton, 2, 2, 1, 1);
  connect(LogSystemFileDirPushButton, &QPushButton::clicked, this,
          &SettingsDialog::logSystemFileDirPushButton_slot);

  LogSystemFileMaxNumberLabel = new QLabel("Максимум лог-файлов");
  LogSystemMainLayout->addWidget(LogSystemFileMaxNumberLabel, 3, 0, 1, 1);
  LogSystemFileMaxNumberLineEdit =
      new QLineEdit(Settings.value("log_system/file_max_number").toString());
  LogSystemMainLayout->addWidget(LogSystemFileMaxNumberLineEdit, 3, 1, 1, 2);
}

void SettingsDialog::createPersoServer() {
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
}

void SettingsDialog::createProgrammator() {
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
}

void SettingsDialog::createStickerPrinter() {
  StickerPrinterGroupBox = new QGroupBox(QString("Стикер-принтер"));
  MainLayout->addWidget(StickerPrinterGroupBox);

  StickerPrinterMainLayout = new QGridLayout();
  StickerPrinterGroupBox->setLayout(StickerPrinterMainLayout);

  StickerPrinterLibPathLabel = new QLabel("Путь к библиотеке");
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathLabel, 0, 0, 1, 1);
  StickerPrinterLibPathLineEdit =
      new QLineEdit(Settings.value("sticker_printer/library_path").toString());
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathLineEdit, 0, 1, 1,
                                      1);
  StickerPrinterLibPathPushButton = new QPushButton("Обзор");
  StickerPrinterMainLayout->addWidget(StickerPrinterLibPathPushButton, 0, 2, 1,
                                      1);
  connect(StickerPrinterLibPathPushButton, &QPushButton::clicked, this,
          &SettingsDialog::stickerPrinterLibPathPushButton_slot);

  StickerPrinterNameLabel = new QLabel("Системное имя");
  StickerPrinterMainLayout->addWidget(StickerPrinterNameLabel, 1, 0, 1, 1);
  StickerPrinterNameLineEdit =
      new QLineEdit(Settings.value("sticker_printer/system_name").toString());
  StickerPrinterMainLayout->addWidget(StickerPrinterNameLineEdit, 1, 1, 1, 2);

  StickerPrinterUseEthernetLabel =
      new QLabel("Использовать Ethernet-подключение");
  StickerPrinterMainLayout->addWidget(StickerPrinterUseEthernetLabel, 2, 0, 1,
                                      1);
  StickerPrinterUseEthernetCheck = new QCheckBox();
  StickerPrinterUseEthernetCheck->setChecked(
      Settings.value("sticker_printer/use_ethernet").toBool());
  StickerPrinterMainLayout->addWidget(StickerPrinterUseEthernetCheck, 2, 1, 2,
                                      1);
  connect(StickerPrinterUseEthernetCheck, &QCheckBox::stateChanged, this,
          &SettingsDialog::stickerPrinterUseEthernetStateChanged_slot);

  createStickerPrinterProxyWidget();
}

void SettingsDialog::createStickerPrinterProxyWidget() {
  StickerPrinterProxyWidget = new QWidget();
  StickerPrinterMainLayout->addWidget(StickerPrinterProxyWidget, 3, 0, 1, 2);

  StickerPrinterProxyWidgetLayout = new QGridLayout();
  StickerPrinterProxyWidget->setLayout(StickerPrinterProxyWidgetLayout);
  StickerPrinterMainLayout->setSizeConstraint(QLayout::SetMinimumSize);
  StickerPrinterMainLayout->setAlignment(Qt::AlignTop);

  StickerPrinterIpLabel = new QLabel("IP адрес");
  StickerPrinterProxyWidgetLayout->addWidget(StickerPrinterIpLabel, 0, 0, 1, 1);
  StickerPrinterIpLineEdit =
      new QLineEdit(Settings.value("sticker_printer/ip_address").toString());
  StickerPrinterProxyWidgetLayout->addWidget(StickerPrinterIpLineEdit, 0, 1, 1,
                                             1);

  StickerPrinterPortLabel = new QLabel("Порт");
  StickerPrinterProxyWidgetLayout->addWidget(StickerPrinterPortLabel, 1, 0, 1,
                                             1);
  StickerPrinterPortLineEdit =
      new QLineEdit(Settings.value("sticker_printer/port").toString());
  StickerPrinterProxyWidgetLayout->addWidget(StickerPrinterPortLineEdit, 1, 1,
                                             1, 1);

  if (!Settings.value("sticker_printer/use_ethernet").toBool()) {
    StickerPrinterProxyWidget->hide();
  }
}

void SettingsDialog::createButtons() {
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
  int32_t msgMaxSize = LogSystemMessageMaxSizeLineEdit->text().toInt();
  QFileInfo info(LogSystemFileDirLineEdit->text());
  int32_t fileMaxNumber = LogSystemFileMaxNumberLineEdit->text().toInt();

  if (msgMaxSize == 0 || fileMaxNumber == 0) {
    return false;
  }

  if (!info.isDir() || !info.isWritable() || !info.isReadable()) {
    return false;
  }

  QHostAddress IP = QHostAddress(PersoServerIpAddressLineEdit->text());
  int32_t port = PersoServerPortLineEdit->text().toInt();
  if (IP.isNull()) {
    return false;
  }

  if ((port > IP_PORT_MAX_VALUE) || (port < IP_PORT_MIN_VALUE)) {
    return false;
  }

  uint32_t speed = ProgrammerSpeedLineEdit->text().toUInt();
  info.setFile(ProgrammerExeFilePathLineEdit->text());
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

  if (StickerPrinterUseEthernetCheck->isChecked()) {
    IP = QHostAddress(PersoServerIpAddressLineEdit->text());
    if (IP.isNull()) {
      return false;
    }

    port = StickerPrinterPortLineEdit->text().toInt();
    if ((port > IP_PORT_MAX_VALUE) || (port < IP_PORT_MIN_VALUE)) {
      return false;
    }
  }
  return true;
}

void SettingsDialog::save() {
  Settings.setValue("log_system/global_enable",
                    LogSystemGlobalEnableCheckBox->isChecked());
  Settings.setValue("log_system/message_max_size",
                    LogSystemMessageMaxSizeLineEdit->text().toInt());
  Settings.setValue("log_system/file_directory",
                    LogSystemFileDirLineEdit->text());
  Settings.setValue("log_system/file_max_number",
                    LogSystemFileMaxNumberLineEdit->text());

  Settings.setValue("perso_server_connection/ip",
                    PersoServerIpAddressLineEdit->text());
  Settings.setValue("perso_server_connection/port",
                    PersoServerPortLineEdit->text().toInt());

  Settings.setValue("jlink_exe_programmer/exe_file_path",
                    ProgrammerExeFilePathLineEdit->text());
  Settings.setValue("jlink_exe_programmer/speed",
                    ProgrammerSpeedLineEdit->text());

  Settings.setValue("sticker_printer/library_path",
                    StickerPrinterLibPathLineEdit->text());
  Settings.setValue("sticker_printer/system_name",
                    StickerPrinterNameLineEdit->text());
  Settings.setValue("sticker_printer/use_ethernet",
                    StickerPrinterUseEthernetCheck->isChecked());
  Settings.setValue("sticker_printer/ip_address",
                    StickerPrinterIpLineEdit->text());
  Settings.setValue("sticker_printer/port", StickerPrinterPortLineEdit->text());
}

void SettingsDialog::logSystemFileDirPushButton_slot() {
  QString filePath =
      QFileDialog::getExistingDirectory(this, "Выберите директорию", "");
  LogSystemFileDirLineEdit->setText(filePath);
}

void SettingsDialog::programmerExeFilePathPushButton_slot() {
  QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "");
  ProgrammerExeFilePathLineEdit->setText(filePath);
}

void SettingsDialog::stickerPrinterLibPathPushButton_slot() {
  QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "");
  StickerPrinterLibPathLineEdit->setText(filePath);
}

void SettingsDialog::stickerPrinterUseEthernetStateChanged_slot(int32_t state) {
  if (state == Qt::Checked) {
    StickerPrinterProxyWidget->show();
    StickerPrinterMainLayout->addWidget(StickerPrinterProxyWidget, 3, 0, 1, 2);
  } else {
    StickerPrinterProxyWidget->hide();
    StickerPrinterMainLayout->removeWidget(StickerPrinterProxyWidget);
  }

  resize(minimumSizeHint());
}
