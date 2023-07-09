#include "jlink_manual_programmer.h"

JLinkManualProgrammer::JLinkManualProgrammer(QObject *parent)
    : InterfaceProgrammer(parent) {

  processingJLinkExePath(DEFAULT_JLINKEXE_FILE_PATH);
}

JLinkManualProgrammer::~JLinkManualProgrammer() {
  delete JLinkExeInfo;
  delete JLinkScriptInfo;
}

void JLinkManualProgrammer::connect() {
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

void JLinkManualProgrammer::load(void) {
  // Проверка на существование прошивки
  if (LoadingFirmware == nullptr) {
    emit logging(
        QString("The firmware file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr)
  {
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

  // Посылаем сигнал о завершении операции
  emit operationFinished();
}

void JLinkManualProgrammer::setLoadingFirmware(QFile *firmware) {
  LoadingFirmware = firmware;
}

void JLinkManualProgrammer::erase() {
  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr)
  {
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

  // Посылаем сигнал о завершении операции
  emit operationFinished();
}

void JLinkManualProgrammer::processingJLinkExePath(const QString &path) {
  JLinkExeInfo = new QFileInfo(path);

  if ((JLinkExeInfo->exists()) && (JLinkExeInfo->isFile()))
  {
    JLinkProcess = new QProcess(this);
    JLinkProcess->setProgram(path);
  }
  else
    JLinkProcess = nullptr;
}

void JLinkManualProgrammer::excuteJLinkScript() {
  // Добавляем завершение скрипта
  JLinkScript->write(QByteArray("r\n"));
  JLinkScript->write(QByteArray("g\n"));
  JLinkScript->write(QByteArray("exit\n"));
  JLinkScript->close();

  // Запускаем JLink.exe
  ProcessArguments << "-CommandFile" << JLinkScript->fileName();
  JLinkProcess->setArguments(ProcessArguments);
  JLinkProcess->start();
  JLinkProcess->waitForFinished();
  ProcessOutput = JLinkProcess->readAllStandardOutput();
  JLinkProcess->close();

  // Логгирование вывода JLink.exe
  emit logging(ProcessOutput);
}

void JLinkManualProgrammer::initScript() {
  // Удаляем старый скрипт для адаптера
  JLinkScriptInfo = new QFileInfo(DEFAULT_JLINK_SCRIPT_FILE_NAME);
  if ((JLinkScriptInfo->exists()) && (JLinkScriptInfo->isFile()))
    QFile::remove(DEFAULT_JLINK_SCRIPT_FILE_NAME);

  // Создаем новый скрипт для адаптера
  JLinkScript = new QFile(DEFAULT_JLINK_SCRIPT_FILE_NAME, this);
  if (JLinkScript->open(QIODevice::WriteOnly)) {
    emit logging("JLink command script created. ");

    // Добавляем иницирующие команды в скрипт
    JLinkScript->write(QByteArray("device N32L403KB\n"));
    JLinkScript->write(QByteArray("if SWD\n"));
    JLinkScript->write(QByteArray("speed 4000\n"));
  } else {
    emit logging("JLink command script creation failed. ");
  }
}
