#include <QObject>
#include <QString>
#include <QTextStream>

#include "file_log_backend.h"
#include "log_backend.h"

FileLogBackend::FileLogBackend(QObject* parent) : LogBackend(parent) {
  setObjectName("FileLogBackend");
  initialize();

  connect(this, &FileLogBackend::notifyAboutError,
          InteractionSystem::instance(),
          &InteractionSystem::generateErrorMessage);
}

FileLogBackend::~FileLogBackend() {}

void FileLogBackend::writeLogLine(const QString& str) {
  if (LogEnable) {
    LogTextStream << str << "\n";
  }
}

void FileLogBackend::clear() { /* No-op */
}

void FileLogBackend::initialize() {
  QDir logDir;
  if (!logDir.mkpath(QApplication::applicationDirPath() + "/logs")) {
    LogEnable = false;
    emit notifyAboutError("Не удалось создать директорию для логгирования. ");
    return;
  }

  CurrentLogDir = QApplication::applicationDirPath() + "/logs/log " +
                  QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm.ss");
  CurrentLogFile.setFileName(CurrentLogDir);
  if (!CurrentLogFile.open(QIODevice::WriteOnly)) {
    LogEnable = false;
    emit notifyAboutError("Не удалось открыть файл для логгирования. ");
    return;
  }

  removeOldestLogFiles();

  LogEnable = true;
  LogTextStream.setDevice(&CurrentLogFile);
}

void FileLogBackend::removeOldestLogFiles() {
  QDir directory(CurrentLogDir);

  // Получаем список файлов в директории
  QFileInfoList fileList =
      directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);

  if (fileList.size() > LOG_FILE_MAX_NUMBER) {
    int filesToDeleteCount = fileList.size() - LOG_FILE_MAX_NUMBER;

    // Удаляем самые старые файлы
    for (int i = 0; i < filesToDeleteCount; ++i) {
      const QFileInfo& fileInfo = fileList.at(i);
      QString filePath = fileInfo.absoluteFilePath();

      QFile::remove(filePath);
    }
  }
}
