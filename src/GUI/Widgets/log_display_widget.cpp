#include "log_display_widget.h"

LogDisplayWidget::LogDisplayWidget(QWidget* parent) {
  create();
}

void LogDisplayWidget::displayLog(const QString& log) {
  if (Display->toPlainText().size() > 500000)
    Display->clear();

  Display->appendPlainText(log);
}

void LogDisplayWidget::clearLogDisplay() {
  Display->clear();
}

void LogDisplayWidget::create() {
  setTitle("Логи");
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
