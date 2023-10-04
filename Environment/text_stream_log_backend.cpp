#include <QObject>
#include <QString>
#include <QTextStream>

#include "log_backend.h"
#include "text_stream_log_backend.h"

TextStreamLogBackend::TextStreamLogBackend(QObject *parent, QTextStream *stream)
 : LogBackend(parent)
{
  LogTextStream = stream;
}

void TextStreamLogBackend::writeLogLine(const QString &str)
{
 *LogTextStream << str << "\n" << Qt::flush;
}

void TextStreamLogBackend::clear() {} /* No-op */
