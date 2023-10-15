#ifndef WIDGETLOGBACKEND_H
#define WIDGETLOGBACKEND_H

#include <QApplication>
#include <QObject>

#include "GUI/master_gui.h"
#include "Log/log_backend.h"

class WidgetLogBackend : public LogBackend {
  Q_OBJECT
 public:
  WidgetLogBackend(QObject* parent);

  virtual void writeLogLine(const QString& str) override;
  virtual void clear() override;

 private:
  Q_DISABLE_COPY(WidgetLogBackend);

 signals:
  void displayLog_signal(const QString& logData);
  void clearLogDisplay_signal(void);
};

#endif /* WIDGETLOGBACKEND_H */
