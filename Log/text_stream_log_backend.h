#ifndef FILELOGBACKEND_H
#define FILELOGBACKEND_H

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QString>
#include <QTextStream>

#include "GUI/interaction_system.h"
#include "log_backend.h"

class TextStreamLogBackend: public LogBackend {
  Q_OBJECT
 private:
  bool LogEnable;
  QFile CurrentLogFile;
  QTextStream LogTextStream;

 public:
  explicit TextStreamLogBackend(QObject* parent);
  ~TextStreamLogBackend();

  virtual void writeLogLine(const QString& str) override;
  virtual void clear() override;

 private:
  Q_DISABLE_COPY(TextStreamLogBackend);
  void initialize();
  void removeOldestLogFiles(void);

 signals:
  void notifyAboutError(const QString& log);
};

#endif /* FILELOGBACKEND_H */
