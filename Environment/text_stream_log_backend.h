#ifndef FILELOGBACKEND_H
#define FILELOGBACKEND_H

#include <QObject>
#include <QTextStream>
#include <QString>

#include "log_backend.h"

class TextStreamLogBackend: public LogBackend {
 private:
  QTextStream *LogTextStream;

 public:
  TextStreamLogBackend(QObject *parent, QTextStream *stream);
  virtual void writeLogLine(const QString &str) override;
  virtual void clear() override;
};

#endif /* FILELOGBACKEND_H */
