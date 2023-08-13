#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <QObject>

class LogSystem : public QObject {
  Q_OBJECT
 private:
  bool IsEnable;

 public:
  explicit LogSystem(QObject* parent);

 public:
  void enable(bool flag);

 public slots:
  void generateLog(const QString& log);

 signals:
  void requestDisplayLog(const QString& log);
};

#endif  // LOG_SYSTEM_H
