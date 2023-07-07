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
  // Даем доступ только одному потоку
  Mutex.lock();

  // Проверка на существование программы адаптера для программатора JLink
  if (JLinkProcess == nullptr) {
    emit logging(
        QString("The JLink.exe file is missing in the default directory, "
                "select it manually."));
    return;
  }
  // Логгирование
  emit logging(QString("Connecting to the device."));
  initScript();
  JLinkScript->write(QByteArray("connect\n"));
  excuteJLinkScript();

  // Посылаем сигнал о завершении операции
  emit operationFinished();

  // Разблокируем доступ
  Mutex.unlock();
}

void JLinkManualProgrammer::load(QFile *firmware) {
  // Даем доступ только одному потоку
  Mutex.lock();

  // Проверка на сущетсвование прошивки
  if (firmware == nullptr) {
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

  initScript();
  JLinkScript->write(QByteArray("erase\n"));
  QString temp =
      QString("loadbin ") + firmware->fileName() + QString(", 0x08000000\n");
  JLinkScript->write(temp.toUtf8());

  excuteJLinkScript();

  // Посылаем сигнал о завершении операции
  emit operationFinished();

  // Разблокируем доступ
  Mutex.unlock();
}

void JLinkManualProgrammer::erase() {
  // Даем доступ только одному потоку
  Mutex.lock();

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

  initScript();
  JLinkScript->write(QByteArray("erase\n"));

  excuteJLinkScript();

  // Посылаем сигнал о завершении операции
  emit operationFinished();

  // Разблокируем доступ
  Mutex.unlock();
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

  ProcessArguments << "-CommandFile" << JLinkScript->fileName();
  JLinkProcess->setArguments(ProcessArguments);
  JLinkProcess->start();
  JLinkProcess->waitForFinished();
  ProcessOutput = JLinkProcess->readAllStandardOutput();
  JLinkProcess->close();

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
