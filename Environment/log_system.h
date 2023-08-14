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
  void setEnable(bool flag);
  void clear(void);

 public slots:
  void generateLog(const QString& log);

 signals:
  void requestDisplayLog(const QString& log);
  void requestClearDisplayLog(void);
};

#endif  // LOG_SYSTEM_H
