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
  JLinkScript->write(QByteArray("erase\n"));
  QString temp = QString("loadbin ") + LoadingFirmware->fileName() +
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
  JLinkScript->write(QByteArray("reset\n"));
  // Запускаем ядро
  JLinkScript->write(QByteArray("go\n"));
  // Выходим из JLink.exe
  JLinkScript->write(QByteArray("exit\n"));

  // Закрываем файл
  JLinkScript->close();

  // Запускаем JLink.exe с соответствующими аргументами
  ProcessArguments << "-nogui"
                   << "1";
  ProcessArguments << "-exitonerror"
                   << "1";
  ProcessArguments << "-device"
                   << "N32L403KB";
  ProcessArguments << "-if"
                   << "SWD";
  ProcessArguments << "-speed"
                   << "4000";
  ProcessArguments << "-jtagconf"
                   << "-1 -1";
  ProcessArguments << "-autoconnect"
                   << "1";
  ProcessArguments << "-commandfile" << JLinkScript->fileName();
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
    JLinkScript->write(QByteArray("USB\n"));
    // Вводим МК в состояние Reset
    //    JLinkScript->write(QByteArray("R0\n"));
    // Подключаемся к МК
    JLinkScript->write(QByteArray("connect\n"));
    // Останавливаем ядро МК
    // JLinkScript->write(QByteArray("Halt\n"));
    // Снимаем состояние Reset
    //    JLinkScript->write(QByteArray("R1\n"));
  } else {
    emit logging("JLink command script creation failed. ");
  }
}
