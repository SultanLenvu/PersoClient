#ifndef FILELOGBACKEND_H
#define FILELOGBACKEND_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QString>

#include "log_backend.h"

class TextStreamLogBackend: public LogBackend {
  Q_OBJECT
  private:
    QFile File;
    QTextStream *LogTextStream;

  public:
    TextStreamLogBackend(QObject *parent, const QString &filename);
    ~TextStreamLogBackend();
    bool initialize();
    virtual void writeLogLine(const QString &str) override;
    virtual void clear() override;
};

#endif /* FILELOGBACKEND_H */
