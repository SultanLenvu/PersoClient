#ifndef WIDGETLOGBACKEND_H
#define WIDGETLOGBACKEND_H

#include <QApplication>
#include <QObject>

#include "log_backend.h"

class WidgetLogBackend : public LogBackend {
  Q_OBJECT
 private:
  bool LogEnable;

 public:
  explicit WidgetLogBackend(const QString& name);
  ~WidgetLogBackend();

  virtual void writeLogLine(const QString& str) override;
  virtual void clear() override;
  virtual void applySettings() override;

 private:
  Q_DISABLE_COPY_MOVE(WidgetLogBackend);
  void loadSettings(void);

 signals:
  void displayLog_signal(const QString& log);
  void clearLogDisplay_signal(void);
};

#endif /* WIDGETLOGBACKEND_H */
