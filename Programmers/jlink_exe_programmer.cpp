#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTemporaryDir>

#include "jlink_exe_programmer.h"
#include "definitions.h"

JLinkExeProgrammer::JLinkExeProgrammer(const QString& name)
    : NamedObject(name), LoggableObject(name) {
  doLoadSettings();
  createJLinkProcess();
}

ReturnStatus JLinkExeProgrammer::checkConfig() {
  sendLog("Проверка конфигурации.");

  QFileInfo info(JLinkPath);
  if (!info.isExecutable()) {
    sendLog("Получен некорректный исполняемый файл JLink.exe.");
    return ReturnStatus::ProgrammatorExeFileInvalid;
  }

  sendLog("Проверка конфигурации успешно завершена.");
  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programMemory(const QByteArray& data) {
  sendLog(QString("Загрузка прошивки."));

  QString fileName =
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME);
  QFile tempFile(fileName);
  if (!tempFile.open(QIODevice::WriteOnly)) {
    sendLog(
        "Не удалось открыть временный файл с данными для загрузки в память.");
    return ReturnStatus::FileOpenError;
  }
  qint64 bytesWritten = tempFile.write(data);
  tempFile.close();

  if (bytesWritten != data.size()) {
    sendLog(
        "Не удалось сохранить данные в временный файл для загрузки в память.");
    return ReturnStatus::FileWriteError;
  }

  // Формируем скрипт JLink
  initScript();
  // Очищаем FLash
  JLinkScript->write(QByteArray("Erase\n"));
  // Загружаем прошивку
  JLinkScript->write(
      QString("LoadFile %1, 0x08000000\n").arg(fileName).toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!tempFile.remove()) {
    sendLog(
        "Не удалось удалить временный файл с данными для загрузки в память.");
  }

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programMemory(const QString& fileName) {
  sendLog(QString("Загрузка прошивки."));

  // Формируем скрипт JLink
  initScript();
  // Очищаем FLash
  JLinkScript->write(QByteArray("Erase\n"));
  // Загружаем прошивку
  JLinkScript->write(
      QString("LoadFile %1, 0x08000000\n").arg(fileName).toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programMemoryWithUnlock(
    const QByteArray& data) {
  sendLog(QString("Разблокировка памяти и загрузка прошивки."));

  QString fileName =
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME);
  QFile tempFile(fileName);
  if (!tempFile.open(QIODevice::WriteOnly)) {
    sendLog(
        "Не удалось открыть временный файл с данными для загрузки в память.");
    return ReturnStatus::FileOpenError;
  }
  qint64 bytesWritten = tempFile.write(data);
  tempFile.close();

  if (bytesWritten != data.size()) {
    sendLog(
        "Не удалось сохранить данные в временный файл для загрузки в память.");
    return ReturnStatus::FileWriteError;
  }

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
  JLinkScript->write(
      QString("LoadFile %1, 0x08000000\n").arg(fileName).toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!tempFile.remove()) {
    sendLog(
        "Не удалось удалить временный файл с данными для загрузки в память.");
  }

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programMemoryWithUnlock(
    const QString& fileName) {
  sendLog(QString("Загрузка прошивки."));

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
  JLinkScript->write(
      QString("LoadFile %1, 0x08000000\n").arg(fileName).toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::readMemory(QByteArray& data) {
  sendLog(QString("Считывание прошивки."));

  QString fileName =
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME);

  // Формируем скрипт JLink
  initScript();
  // Считываем всю flash-память
  JLinkScript->write(
      QByteArray("savebin saved_firmware.bin, 0x08000000, 0x10000\n"));

  // Запускаем выполнение скрипта JLink
  executeJLinkScript();

  // Запускаем выполнение скрипта JLink
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  // Считываем данные из файла
  QFile tempFile(fileName);
  if (!tempFile.open(QIODevice::ReadOnly)) {
    sendLog("Не удалось открыть временный файл.");
    return ReturnStatus::FileOpenError;
  }

  data.reserve(tempFile.size());
  data = tempFile.readAll();
  if (!tempFile.remove()) {
    sendLog(
        "Не удалось удалить временный файл с данными, считанными из памяти.");
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::readMemory(const QString& fileName) {
  sendLog(QString("Считывание прошивки."));

  // Формируем скрипт JLink
  initScript();
  // Считываем всю flash-память
  JLinkScript->write(
      QString("savebin %1, 0x08000000, 0x10000\n").arg(fileName).toUtf8());

  // Запускаем выполнение скрипта JLink
  executeJLinkScript();

  // Запускаем выполнение скрипта JLink
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::eraseMemory() {
  sendLog(QString("Стирание прошивки."));

  // Формируем скрипт JLink
  initScript();
  JLinkScript->write(QByteArray("erase\n"));

  // Запускаем выполнение скрипта JLink
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::readUserData(QByteArray& data) {
  sendLog(QString("Чтение пользовательских данных."));

  QString fileName =
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME);

  // Формируем скрипт JLink
  initScript();
  // Считываем часть flash-памяти, в которой хранятся пользовательские данные
  JLinkScript->write(QString("savebin %1, %2, %3\n")
                         .arg(fileName, TRANSPONDER_USER_DATA_START_ADDRESS,
                              QString::number(TRANSPONDER_USER_DATA_SIZE))
                         .toUtf8());

  // Запускаем выполнение скрипта JLink
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  // Считываем данные из файла
  QFile tempFile(fileName);
  if (!tempFile.open(QIODevice::ReadOnly)) {
    sendLog("Не удалось открыть временный файл.");
    return ReturnStatus::FileOpenError;
  }

  data.reserve(tempFile.size());
  data = tempFile.readAll();
  if (!tempFile.remove()) {
    sendLog(
        "Не удалось удалить временный файл с данными, считанными из памяти.");
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::readUserData(const QString& fileName) {
  sendLog(QString("Чтение пользовательских данных."));

  // Формируем скрипт JLink
  initScript();
  // Считываем часть flash-памяти, в которой хранятся пользовательские данные
  JLinkScript->write(QString("savebin %1, %2, %3\n")
                         .arg(fileName, TRANSPONDER_USER_DATA_START_ADDRESS,
                              QString::number(TRANSPONDER_USER_DATA_SIZE))
                         .toUtf8());

  // Запускаем выполнение скрипта JLink
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programUserData(const QByteArray& data) {
  sendLog(QString("Запись пользовательских данных."));

  QString fileName =
      QString("%1/%2").arg(QDir::tempPath(), TRANSPONDER_FIRMWARE_FILE_NAME);
  QFile tempFile(fileName);
  if (!tempFile.open(QIODevice::WriteOnly)) {
    sendLog(
        "Не удалось открыть временный файл с данными для загрузки в память.");
    return ReturnStatus::FileOpenError;
  }
  qint64 bytesWritten = tempFile.write(data);
  tempFile.close();

  if (bytesWritten != data.size()) {
    sendLog(
        "Не удалось сохранить данные в временный файл для загрузки в память.");
    return ReturnStatus::FileWriteError;
  }

  // Формируем скрипт JLink
  initScript();

  // Очищаем старые пользовательские данные
  JLinkScript->write(QString("Erase %1, %2\n")
                         .arg(TRANSPONDER_USER_DATA_START_ADDRESS,
                              TRANSPONDER_USER_DATA_END_ADDRESS)
                         .toUtf8());
  // Загружаем новые пользовательских данных
  JLinkScript->write(QString("LoadFile %1, %2\n")
                         .arg(fileName, TRANSPONDER_USER_DATA_START_ADDRESS)
                         .toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!tempFile.remove()) {
    sendLog(
        "Не удалось удалить временный файл с данными для загрузки в память.");
  }

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::programUserData(
    const QString& fileName) {  // Формируем скрипт JLink
  initScript();

  // Очищаем старые пользовательские данные
  JLinkScript->write(QString("Erase %1, %2\n")
                         .arg(TRANSPONDER_USER_DATA_START_ADDRESS,
                              TRANSPONDER_USER_DATA_END_ADDRESS)
                         .toUtf8());
  // Загружаем новые пользовательских данных
  JLinkScript->write(QString("LoadFile %1, %2\n")
                         .arg(fileName, TRANSPONDER_USER_DATA_START_ADDRESS)
                         .toUtf8());

  // Запускаем выполнение скрипта JLink
  bool ok = executeJLinkScript();

  if (!ok) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::readTransponderUcid(QString& ucid) {
  sendLog(QString("Считывание UCID микроконтроллера."));

  QString ucidFileName;
  ucidFileName = QString("%1/ucid").arg(QDir::tempPath());

  // Формируем скрипт JLink
  initScript();
  // Формируем сценарий команд
  JLinkScript->write(QByteArray(QString("savebin %1, %2, %3\n")
                                    .arg(ucidFileName, UCID_MEMORY_ADDRESS,
                                         QString::number(UCID_SIZE, 16))
                                    .toUtf8()));

  // Запускаем выполнение скрипта JLink

  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  QByteArray ucidByteArray;
  QFile ucidFile(ucidFileName);
  if (!ucidFile.open(QIODevice::ReadOnly)) {
    sendLog("Не удалось открыть файл содержащий UCID.");
    return ReturnStatus::FileOpenError;
  }
  ucidByteArray = ucidFile.readAll();

  ucid = ucidByteArray.toHex();

  ucidFile.close();
  ucidFile.remove();

  sendLog(QString("Считанный UCID:%1.").arg(ucid));
  return ReturnStatus::NoError;
}

ReturnStatus JLinkExeProgrammer::unlockMemory() {
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
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus
JLinkExeProgrammer::lockMemory() {  // Проверка на существование программы
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
  if (!executeJLinkScript()) {
    return ReturnStatus::ProgrammatorCommandScriptError;
  }

  return ReturnStatus::NoError;
}

/*
 * Приватные методы
 */

void JLinkExeProgrammer::loadSettings() {
  sendLog("Загрузка настроек.");
  doLoadSettings();
  createJLinkProcess();
}

void JLinkExeProgrammer::doLoadSettings() {
  QSettings settings;

  JLinkPath = settings.value("jlink_exe_programmer/exe_file_path").toString();
  Speed = settings.value("jlink_exe_programmer/speed").toUInt();
}

void JLinkExeProgrammer::createJLinkProcess() {
  JLinkProcess = std::unique_ptr<QProcess>(new QProcess());
  JLinkProcess->setProgram(JLinkPath);
}

bool JLinkExeProgrammer::executeJLinkScript() {
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
  if (!JLinkProcess->waitForFinished()) {
    sendLog("Получен некорректный исполняемый файл JLink.exe.");
    JLinkProcess->close();
    return false;
  }

  if ((JLinkProcess->exitStatus() != QProcess::NormalExit) ||
      (JLinkProcess->exitCode() != 0)) {
    sendLog(JLinkProcess->readAllStandardOutput());
    JLinkProcess->close();
    return false;
  }

  JLinkProcess->close();
  sendLog("Скрипт успешно выполнен.");
  return true;
}

bool JLinkExeProgrammer::executeJLinkScript(QString& output) {
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
  if (!JLinkProcess->waitForFinished()) {
    sendLog("Получен некорректный исполняемый файл JLink.exe.");
    JLinkProcess->close();
    return false;
  }
  output = JLinkProcess->readAllStandardOutput();
  JLinkProcess->close();

  if ((JLinkProcess->exitStatus() != QProcess::NormalExit) ||
      (JLinkProcess->exitCode() != 0)) {
    sendLog(output);
    return false;
  }

  sendLog("Скрипт успешно выполнен.");
  return true;
}

void JLinkExeProgrammer::initScript() {
  // Удаляем старый скрипт для адаптера
  QFileInfo info(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);
  if ((info.exists()) && (info.isFile())) {
    QFile::remove(JLINK_COMMAND_SCRIPT_DEFAULT_NAME);
  }

  // Создаем новый скрипт для адаптера
  JLinkScript =
      std::unique_ptr<QFile>(new QFile(JLINK_COMMAND_SCRIPT_DEFAULT_NAME));
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
