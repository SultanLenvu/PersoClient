#ifndef WIDGETLOGBACKEND_H
#define WIDGETLOGBACKEND_H

#include <QObject>

#include "Environment/log_backend.h"
#include "GUI/master_gui.h"

class WidgetLogBackend: public LogBackend {
  public:
    WidgetLogBackend(QObject *parent, MasterGUI *gui);
    ~WidgetLogBackend();

    void writeLogLine(const QString &str) override;
    void clear() override;

  signals:
    void requestDisplayLog(const QString& logData);
    void requestClearDisplayLog(void);
};

#endif /* WIDGETLOGBACKEND_H */
