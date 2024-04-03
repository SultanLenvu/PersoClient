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

 private:
  Q_DISABLE_COPY_MOVE(LogDisplayWidget)
  void create(void);
  void connectDepedencies(void);

 private slots:
  void displayLog(const QString& log);
  void clearLogDisplay(void);
};

#endif  // LOGDISPLAYWIDGET_H
