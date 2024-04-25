#ifndef LOGDISPLAYWIDGET_H
#define LOGDISPLAYWIDGET_H

#include <QGroupBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>

class LogDisplayWidget : public QGroupBox {
  Q_OBJECT
 private:
  QVBoxLayout* Layout;
  QPlainTextEdit* Display;

 public:
  explicit LogDisplayWidget(QWidget* parent = nullptr);
  ~LogDisplayWidget(void) = default;
  Q_DISABLE_COPY_MOVE(LogDisplayWidget)

 public slots:
  void displayLog(const QString& log);
  void clearLogDisplay(void);

 private:
  void create(void);
};

#endif  // LOGDISPLAYWIDGET_H
