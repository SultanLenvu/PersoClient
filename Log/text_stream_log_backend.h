#ifndef FILELOGBACKEND_H
#define FILELOGBACKEND_H

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTextStream>

#include "log_backend.h"

class TextStreamLogBackend: public LogBackend {
  Q_OBJECT
 private:
  bool LogEnable;
  QFile File;
  QTextStream LogTextStream;

 public:
  explicit TextStreamLogBackend(QObject* parent);
  ~TextStreamLogBackend();

  virtual void writeLogLine(const QString& str) override;
  virtual void clear() override;

 private:
  Q_DISABLE_COPY(TextStreamLogBackend);
  void initialize();
};

#endif /* FILELOGBACKEND_H */
