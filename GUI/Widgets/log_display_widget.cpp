#include "log_display_widget.h"
#include "global_environment.h"
#include "widget_log_backend.h"

LogDisplayWidget::LogDisplayWidget(QWidget* parent) {
  create();
  connectDepedencies();
}

void LogDisplayWidget::create() {
  Layout = new QVBoxLayout();
  setLayout(Layout);

  Display = new QPlainTextEdit();
  Display->setObjectName(QString::fromUtf8("Display"));
  Display->setEnabled(true);
  Display->setTabletTracking(true);
  Display->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  Display->setCenterOnScroll(false);
  Layout->addWidget(Display);
}

void LogDisplayWidget::connectDepedencies() {
  WidgetLogBackend* wlb =
      GlobalEnvironment::instance()->getObject<WidgetLogBackend>(
          "WidgetLogBackend");

  connect(wlb, &WidgetLogBackend::displayLog_signal, this,
          &LogDisplayWidget::displayLog);
  connect(wlb, &WidgetLogBackend::clearLogDisplay_signal, this,
          &LogDisplayWidget::clearLogDisplay);
}

void LogDisplayWidget::displayLog(const QString& log) {
  if (Display->toPlainText().size() > 500000)
    Display->clear();

  Display->appendPlainText(log);
}

void LogDisplayWidget::clearLogDisplay() {
  Display->clear();
}
