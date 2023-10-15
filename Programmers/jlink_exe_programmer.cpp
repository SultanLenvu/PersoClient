#include "jlink_exe_programmer.h"

JLinkExeProgrammer::JLinkExeProgrammer(QObject* parent)
    : IProgrammer(parent, JLink) {
  JLinkProcess = nullptr;
  loadSettings();
}

JLinkExeProgrammer::~JLinkExeProgrammer() {
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::getUcid(QString* ucid) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Считывание UCID микроконтроллера."));

  // Формируем скрипт JLink
  initScript();
  // Формируем сценарий команд
  JLinkScript->write(
      QByteArray(QString("mem %1, %2\n")
                     .arg(UCID_START_ADDRESS, QString::number(UCID_SIZE, 16))
                     .toUtf8()));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf("Script processing completed.") == -1) {
    ucid->clear();
    return ProgrammatorError;
  }

  for (int32_t i = 0; i < ProcessOutput.size(); i++) {
    if (ProcessOutput.at(i).contains(
            QString(UCID_START_ADDRESS).remove("0x"))) {
      *ucid = ProcessOutput.at(i).mid(11, 50);
      ucid->remove(' ');
      break;
    }
  }

  return Completed;
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::loadFirmware(
    QFile* firmware) {
  // Проверка корректности присланной прошивки
  if (!checkFirmwareFile(firmware)) {
    sendLog(QString("Получен некорректный файл прошивки. Сброс. "));
    return FirmwareFileError;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Загрузка прошивки."));

  // Формируем скрипт JLink
  initScript();
  // Очищаем FLash
  JLinkScript->write(QByteArray("Erase\n"));
  // Загружаем прошивку
  QString temp =
      QString("LoadFile ") + firmware->fileName() + QString(", 0x08000000\n");
  JLinkScript->write(temp.toUtf8());

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if ((ProcessOutput.indexOf("O.K.") > -1) &&
      (ProcessOutput.indexOf("Erasing done.") > -1)) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::loadFirmwareWithUnlock(
    QFile* firmware) {
  // Проверка корректности присланной прошивки
  if (!checkFirmwareFile(firmware)) {
    sendLog(QString("Получен некорректный файл прошивки. Сброс. "));
    return FirmwareFileError;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Разблокировка памяти и загрузка прошивки."));

  // Формируем скрипт JLink
  initScript();

  // Снимаем защиту с flash-памяти
  JLinkScript->write(QByteArray("w4 0x40022004, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022004, 0xCDEF89AB\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0xCDEF89AB\n"));

  JLinkScript->write(QByteArray("w4 0x1FFFF800, 0x00FF5AA5\n"));
  JLinkScript->write(QByteArray("r\n"));
  JLinkScript->write(QByteArray("halt\n"));
  JLinkScript->write(QByteArray("mem 0x1FFFF800, 4\n"));
  JLinkScript->write(QByteArray("connect\n"));

  // Очищаем FLash
  JLinkScript->write(QByteArray("erase\n"));

  // Загружаем прошивку
  QString temp =
      QString("LoadFile ") + firmware->fileName() + QString(", 0x08000000\n");
  JLinkScript->write(temp.toUtf8());

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if ((ProcessOutput.indexOf("O.K.") > -1) &&
      (ProcessOutput.indexOf("Erasing done.") > -1) &&
      (ProcessOutput.indexOf("1FFFF800 = A5 5A FF 00                           "
                             "            .Z..") > -1)) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::readFirmware(void) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Считывание прошивки."));

  // Формируем скрипт JLink
  initScript();
  // Считываем всю flash-память
  JLinkScript->write(
      QByteArray("savebin saved_firmware.bin, 0x08000000, 0x10000\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf(
          "Reading 65536 bytes from addr 0x08000000 into file...O.K.") > -1) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::eraseFirmware() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Стирание прошивки."));

  // Формируем скрипт JLink
  initScript();
  JLinkScript->write(QByteArray("erase\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf("Erasing done.") > -1)
    return Completed;
  else
    return ProgrammatorError;
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::readData(void) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Чтение данных."));

  // Формируем скрипт JLink
  initScript();
  // Считываем часть flash-памяти, в которой хранятся пользовательские данные
  QString temp = QString("savebin saved_user_data.bin,") +
                 QString(USER_DATA_FLASH_START_ADDRESS) + QString(", ") +
                 QString::number(USER_DATA_FLASH_SIZE, 16) + QString("\n");
  JLinkScript->write(temp.toUtf8());

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf(QString("Reading ") +
                            QString::number(USER_DATA_FLASH_SIZE, 10) +
                            QString(" bytes from addr ") +
                            QString(USER_DATA_FLASH_START_ADDRESS) +
                            QString(" into file...O.K.")) > -1) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::loadData(QFile* data) {
  // Проверка корректности присланной прошивки
  if (!checkDataFile(data)) {
    sendLog(QString("Получен некорректный файл с данными. Сброс. "));
    return DataFileError;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Формируем скрипт JLink
  initScript();

  // Очищаем старые пользовательские данные
  QString temp = QString("Erase ") + QString(USER_DATA_FLASH_START_ADDRESS) +
                 QString(", ") + QString(USER_DATA_FLASH_END_ADDRESS) +
                 QString("\n");
  JLinkScript->write(temp.toUtf8());
  // Загружаем новые пользовательских данных
  temp = QString("LoadFile ") + data->fileName() + QString(", ") +
         QString(USER_DATA_FLASH_START_ADDRESS) + QString("\n");
  JLinkScript->write(temp.toUtf8());

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if ((ProcessOutput.indexOf("O.K.") > -1) &&
      (ProcessOutput.indexOf("Erasing done.") > -1)) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus JLinkExeProgrammer::unlockDevice() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Разблокирование памяти."));

  // Формируем скрипт JLink
  initScript();
  // Снимаем защиту с flash-памяти
  JLinkScript->write(QByteArray("w4 0x40022004, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022004, 0xCDEF89AB\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0xCDEF89AB\n"));

  JLinkScript->write(QByteArray("w4 0x1FFFF800, 0x00FF5AA5\n"));
  JLinkScript->write(QByteArray("r\n"));
  JLinkScript->write(QByteArray("mem 0x1FFFF800, 4\n"));
  JLinkScript->write(QByteArray("g\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf("1FFFF800 = A5 5A FF 00                           "
                            "            .Z..") > -1) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

JLinkExeProgrammer::ReturnStatus
JLinkExeProgrammer::lockDevice() {  // Проверка на существование программы
  // адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    sendLog(QString("Отсутсвует JLink.exe. Сброс."));
    return DriverMissing;
  }

  // Логгирование
  sendLog(QString("Блокировка памяти."));

  // Формируем скрипт JLink
  initScript();
  // Включаем защиту flash-памяти
  JLinkScript->write(QByteArray("w4 0x40022004, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022004, 0xCDEF89AB\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0x45670123\n"));
  JLinkScript->write(QByteArray("w4 0x40022008, 0xCDEF89AB\n"));

  JLinkScript->write(QByteArray("w4 0x1FFFF800, 0x00FFFF00\n"));
  JLinkScript->write(QByteArray("r\n"));
  JLinkScript->write(QByteArray("mem 0x1FFFF800, 4\n"));
  JLinkScript->write(QByteArray("g\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf(
          "1FFFF800 = 00 FF FF 00                                       ....") >
      -1) {
    return Completed;
  } else {
    return ProgrammatorError;
  }
}

void JLinkExeProgrammer::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();
}

void JLinkExeProgrammer::sendLog(const QString& log) {
  if (LogEnable) {
    emit logging(QString("%1 - %2").arg(objectName(), log));
  }
}

/*
 * Приватные методы
 */

void JLinkExeProgrammer::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
  ExtendedLoggingEnable = settings.value("log_system/extended_enable").toBool();

  delete JLinkProcess;
  JLinkProcess = new QProcess(this);
  JLinkProcess->setProgram(
      settings.value("jlink_exe_programmer/exe_file_path").toString());

  Speed = settings.value("jlink_exe_programmer/speed").toUInt();
}

void JLinkExeProgrammer::excuteJLinkScript() {
  // Добавляем завершение скрипта
  // Посылаем сигнал Reset на МК
  JLinkScript->write(QByteArray("r\n"));
  // Запускаем ядро
  JLinkScript->write(QByteArray("g\n"));
  // Выходим из JLink.exe
  JLinkScript->write(QByteArray("q\n"));

  // Закрываем файл
  JLinkScript->close();

  // Запускаем JLink.exe с соответствующими аргументами
  ProcessArguments << "-NoGUI"
                   << "1";
  ProcessArguments << "-ExitOnError"
                   << "1";
  ProcessArguments << "-CommandFile" << JLinkScript->fileName();
  JLinkProcess->setArguments(ProcessArguments);
  JLinkProcess->start();
  JLinkProcess->waitForFinished();
  QByteArray rawOutput = JLinkProcess->readAllStandardOutput();
  JLinkProcess->close();

  // Форматирование вывода JLink.exe
  ProcessOutput = QString(rawOutput).split("\r\n");

  // Логгирование вывода JLink.exe
  if (ExtendedLoggingEnable == true) {
    sendLog(rawOutput);
  }
}

void JLinkExeProgrammer::initScript() {
  // Удаляем старый скрипт для адаптера
  QFileInfo info(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);
  if ((info.exists()) && (info.isFile())) {
    QFile::remove(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);
  }

  // Создаем новый скрипт для адаптера
  JLinkScript = new QFile(JLINK_COMMAND_SCRIPT_DEFAULT_NAME, this);
  if (JLinkScript->open(QIODevice::WriteOnly)) {
    sendLog("Командный скрипт JLink создан. ");

    // Добавляем иницирующие команды в скрипт

    // Подключаемся к программатору по USB
    JLinkScript->write("usb\n");
    JLinkScript->write("device N32L403KB\n");
    JLinkScript->write("si SWD\n");
    JLinkScript->write(
        QString("speed %1\n").arg(QString::number(Speed)).toUtf8());
    // Подключаемся к МК: сбрасываем и останавливаем ядро
    JLinkScript->write("r\n");
    JLinkScript->write("halt\n");
    JLinkScript->write("connect\n");
  } else {
    sendLog("Не удалось создать командный скрипт JLink. ");
  }
}
