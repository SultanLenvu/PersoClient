#ifndef WIDGETLOGBACKEND_H
#define WIDGETLOGBACKEND_H

#include <QObject>

#include "Environment/log_backend.h"
#include "GUI/master_gui.h"

class WidgetLogBackend: public LogBackend {
  Q_OBJECT
  public:
    WidgetLogBackend(QObject *parent, MasterGUI *gui);

    virtual void writeLogLine(const QString &str) override;
    virtual void clear() override;

  signals:
    void requestDisplayLog(const QString& logData);
    void requestClearDisplayLog(void);
};

#endif /* WIDGETLOGBACKEND_H */
