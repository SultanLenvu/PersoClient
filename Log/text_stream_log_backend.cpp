#include <QObject>
#include <QString>
#include <QTextStream>
#include <QIODevice>

#include "log_backend.h"
#include "text_stream_log_backend.h"

TextStreamLogBackend::TextStreamLogBackend(QObject *parent,
    const QString &filename)
  : LogBackend(parent), File(filename), LogTextStream(nullptr) {}

TextStreamLogBackend::~TextStreamLogBackend() {
  delete LogTextStream;
}

void TextStreamLogBackend::writeLogLine(const QString &str) {
  *LogTextStream << str << "\n" << Qt::flush;
}

void TextStreamLogBackend::clear() {} /* No-op */

bool TextStreamLogBackend::initialize() {
  if (!File.open(QIODevice::Append | QIODevice::Text))
    return false;
  LogTextStream = new QTextStream(&File);
  return true;
}
