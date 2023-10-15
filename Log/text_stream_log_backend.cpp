#include <QObject>
#include <QString>
#include <QTextStream>

#include "log_backend.h"
#include "text_stream_log_backend.h"

TextStreamLogBackend::TextStreamLogBackend(QObject* parent)
    : LogBackend(parent) {
  setObjectName("TextStreamLogBackend");
  initialize();
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
    QMessageBox::critical(nullptr, "Ошибка",
                          "Не удалось создать директорию для логгирования. ",
                          QMessageBox::Ok);
    return;
  }

  File.setFileName(
      QApplication::applicationDirPath() + "/logs/log " +
      QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm.ss"));
  if (!File.open(QIODevice::WriteOnly)) {
    LogEnable = false;
    QMessageBox::critical(nullptr, "Ошибка",
                          "Не удалось открыть файл для логгирования. ",
                          QMessageBox::Ok);
    return;
  }

  LogEnable = true;
  LogTextStream.setDevice(&File);
}
