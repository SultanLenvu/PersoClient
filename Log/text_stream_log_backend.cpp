#include <QObject>
#include <QString>
#include <QTextStream>

#include "log_backend.h"
#include "text_stream_log_backend.h"

TextStreamLogBackend::TextStreamLogBackend(QObject* parent)
    : LogBackend(parent) {
  setObjectName("TextStreamLogBackend");
  initialize();

  connect(this, &TextStreamLogBackend::notifyAboutError,
          InteractionSystem::instance(),
          &InteractionSystem::generateErrorMessage);
}

TextStreamLogBackend::~TextStreamLogBackend() {
}

void TextStreamLogBackend::writeLogLine(const QString& str) {
  if (LogEnable) {
    LogTextStream << str << "\n";
  }
}

void TextStreamLogBackend::clear() { /* No-op */
}

void TextStreamLogBackend::initialize() {
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

void TextStreamLogBackend::removeOldestLogFiles() {
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
