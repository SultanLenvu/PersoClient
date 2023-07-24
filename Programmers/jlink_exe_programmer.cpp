#include "jlink_exe_programmer.h"

JLinkExeProgrammer::JLinkExeProgrammer(QObject *parent)
    : InterfaceProgrammer(parent) {
  processingJLinkExePath(DEFAULT_JLINKEXE_FILE_PATH);
}

JLinkExeProgrammer::~JLinkExeProgrammer() {
  delete JLinkExeInfo;
  delete JLinkScriptInfo;
}

void JLinkExeProgrammer::connectDevice() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(
        QString("The JLink.exe file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Логгирование
  emit logging(QString("Connecting to the device."));

  // Формируем скрипт JLink
  initScript();
  JLinkScript->write(QByteArray("connect\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Посылаем сигнал о завершении операции
  emit operationFinished();
}

void JLinkExeProgrammer::loadFirmware() { // Проверка на существование прошивки
  if (LoadingFirmware == nullptr) {
    emit logging(
        QString("The firmware file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(
        QString("The JLink.exe file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Логгирование
  emit logging(QString("Program firmware."));

  // Формируем скрипт JLink
  initScript();
  // Очищаем FLash
  JLinkScript->write(QByteArray("Erase\n"));
  // Загружаем прошивку
  QString temp = QString("LoadBin ") + LoadingFirmware->fileName() +
                 QString(", 0x08000000\n");
  JLinkScript->write(temp.toUtf8());

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if ((ProcessOutput.indexOf("O.K.") > -1) &&
      (ProcessOutput.indexOf("Erasing done.") > -1))
    emit operationCompleted();
  else
    emit operationFailed();

  // Посылаем сигнал о завершении операции
  emit operationFinished();
}

void JLinkExeProgrammer::eraseFirmware() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(
        QString("The JLink.exe file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Логгирование
  emit logging(QString("Erase firmware."));

  // Формируем скрипт JLink
  initScript();
  JLinkScript->write(QByteArray("erase\n"));

  // Запускаем выполнение скрипта JLink
  excuteJLinkScript();

  // Обрабатываем вывод JLink.exe
  if (ProcessOutput.indexOf("Erasing done.") > -1)
    emit operationCompleted();
  else
    emit operationFailed();

  // Посылаем сигнал о завершении операции
  emit operationFinished();
}

void JLinkExeProgrammer::resetDevice() {}

void JLinkExeProgrammer::runDevice() {}

void JLinkExeProgrammer::exit() {}

void JLinkExeProgrammer::setLoadingFirmware(QFile *firmware) {
  LoadingFirmware = firmware;
}

void JLinkExeProgrammer::processingJLinkExePath(const QString &path) {
  JLinkExeInfo = new QFileInfo(path);

  if ((JLinkExeInfo->exists()) && (JLinkExeInfo->isFile())) {
    JLinkProcess = new QProcess(this);
    JLinkProcess->setProgram(path);
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
  JLinkScriptInfo = new QFileInfo(DEFAULT_JLINK_SCRIPT_FILE_NAME);
  if ((JLinkScriptInfo->exists()) && (JLinkScriptInfo->isFile()))
    QFile::remove(DEFAULT_JLINK_SCRIPT_FILE_NAME);

  // Создаем новый скрипт для адаптера
  JLinkScript = new QFile(DEFAULT_JLINK_SCRIPT_FILE_NAME, this);
  if (JLinkScript->open(QIODevice::WriteOnly)) {
    emit logging("JLink command script created. ");

    // Добавляем иницирующие команды в скрипт

    // Подключаемся к программатору по USB
    JLinkScript->write(QByteArray("usb\n"));
    JLinkScript->write(QByteArray("device N32L403KB\n"));
    JLinkScript->write(QByteArray("si SWD\n"));
    JLinkScript->write(QByteArray("speed 4000\n"));
    // Подключаемся к МК: сбрасываем и останавливаем ядро
    JLinkScript->write(QByteArray("r\n"));
    JLinkScript->write(QByteArray("halt\n"));
    // Снимаем защиту с flash-памяти
    JLinkScript->write(QByteArray("w4 0x40022004, 0x45670123\n"));
    JLinkScript->write(QByteArray("w4 0x40022004, 0xCDEF89AB\n"));
    JLinkScript->write(QByteArray("w4 0x40022008, 0x45670123\n"));
    JLinkScript->write(QByteArray("w4 0x40022008, 0xCDEF89AB\n"));

    JLinkScript->write(QByteArray("w4 0x1FFFF800, 0x00FF5AA5\n"));
    JLinkScript->write(QByteArray("r\n"));
    JLinkScript->write(QByteArray("mem 0x1FFFF800, 4\n"));
    // JLinkScript->write(QByteArray("g\n"));
    JLinkScript->write(QByteArray("connect\n"));
  } else {
    emit logging("JLink command script creation failed. ");
  }
}
