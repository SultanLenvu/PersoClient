#ifndef SERVERCOMMANDSWIDGET_H
#define SERVERCOMMANDSWIDGET_H

#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

class ServerCommandsWidget : public QGroupBox {
  Q_OBJECT

 private:
  QVBoxLayout* Layout;
  QComboBox* ComboBox;
  QPushButton* Button;

 public:
  explicit ServerCommandsWidget(QWidget* parent = nullptr);
  ~ServerCommandsWidget() = default;

 private:
  void create(void);
  void connectDependencies(void);

 private slots:
  void prepareCommandExecution(void);

 signals:
  void executeCommand_signal(const QString& name);
};

#endif  // SERVERCOMMANDSWIDGET_H
