#include "jlink_exe_programmer.h"

JLinkExeProgrammer::JLinkExeProgrammer(QObject* parent)
    : IProgrammer(parent, JLink) {
  loadSettings();
}

JLinkExeProgrammer::~JLinkExeProgrammer() {
}

void JLinkExeProgrammer::getUcid(QString* ucid) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Считывание UCID микроконтроллера."));

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
    emit operationFinished(ProgrammatorError);
    return;
  }

  for (int32_t i = 0; i < ProcessOutput.size(); i++) {
    if (ProcessOutput.at(i).contains(
            QString(UCID_START_ADDRESS).remove("0x"))) {
      *ucid = ProcessOutput.at(i).mid(11, 50);
      ucid->remove(' ');
      break;
    }
  }

  emit operationFinished(CompletedSuccessfully);
}

void JLinkExeProgrammer::loadFirmware(QFile* firmware) {
  // Проверка корректности присланной прошивки
  if (!checkFirmwareFile(firmware)) {
    emit logging(QString("Получен некорректный файл прошивки. Сброс. "));
    emit operationFinished(RequestParameterError);
    return;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Загрузка прошивки."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::loadFirmwareWithUnlock(QFile* firmware) {
  // Проверка корректности присланной прошивки
  if (!checkFirmwareFile(firmware)) {
    emit logging(QString("Получен некорректный файл прошивки. Сброс. "));
    emit operationFinished(RequestParameterError);
    return;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Разблокировка памяти и загрузка прошивки."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::readFirmware(void) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Считывание прошивки."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::eraseFirmware() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Стирание прошивки."));

  // Формируем скрипт JLink
  initScript();
  JLinkScript->write(QByteArray("erase\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf("Erasing done.") > -1)
    emit operationFinished(CompletedSuccessfully);
  else
    emit operationFinished(ProgrammatorError);
}

void JLinkExeProgrammer::readData(void) {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Чтение данных."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::loadData(QFile* data) {
  // Проверка корректности присланной прошивки
  if (!checkDataFile(data)) {
    emit logging(QString("Получен некорректный файл с данными. Сброс. "));
    emit operationFinished(DataFileError);
    return;
  }

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::unlockDevice() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Разблокирование памяти."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::lockDevice() { // Проверка на существование программы
                                        // адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(QString("Отсутсвует JLink.exe. Сброс."));
    emit operationFinished(ProgrammatorError);
    return;
  }

  // Логгирование
  emit logging(QString("Блокировка памяти."));

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
    emit operationFinished(CompletedSuccessfully);
  } else {
    emit operationFinished(ProgrammatorError);
  }
}

void JLinkExeProgrammer::applySettings() {
  emit logging("Применение новых настроек. ");
  loadSettings();
}

/*
  Приватные методы
*/

void JLinkExeProgrammer::loadSettings() {
  QSettings settings;
  QFileInfo info(settings.value("JLinkExeProgrammer/ExeFile/Path").toString());

  if ((info.exists()) && (info.isFile())) {
    JLinkProcess = new QProcess(this);
    JLinkProcess->setProgram(
        settings.value("JLinkExeProgrammer/ExeFile/Path").toString());
  } else
    JLinkProcess = nullptr;
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
  emit logging(rawOutput);
}

void JLinkExeProgrammer::initScript() {
  // Удаляем старый скрипт для адаптера
  QFileInfo info(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);
  if ((info.exists()) && (info.isFile()))
    QFile::remove(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);

  // Создаем новый скрипт для адаптера
  JLinkScript = new QFile(JLINK_COMMAND_SCRIPT_DEFAULT_NAME, this);
  if (JLinkScript->open(QIODevice::WriteOnly)) {
    emit logging("Командный скрипт JLink создан. ");

    // Добавляем иницирующие команды в скрипт

    // Подключаемся к программатору по USB
    JLinkScript->write(QByteArray("usb\n"));
    JLinkScript->write(QByteArray("device N32L403KB\n"));
    JLinkScript->write(QByteArray("si SWD\n"));
    JLinkScript->write(QByteArray("speed 4000\n"));
    // Подключаемся к МК: сбрасываем и останавливаем ядро
    JLinkScript->write(QByteArray("r\n"));
    JLinkScript->write(QByteArray("halt\n"));
    JLinkScript->write(QByteArray("connect\n"));
  } else {
    emit logging("Не удалось создать командный скрипт JLink. ");
  }
}
