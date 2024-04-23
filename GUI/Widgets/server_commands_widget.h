#ifndef SERVERCOMMANDSWIDGET_H
#define SERVERCOMMANDSWIDGET_H

#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

class ServerCommandsWidget final : public QGroupBox {
  Q_OBJECT

 private:
  QVBoxLayout* Layout;
  QComboBox* ComboBox;
  QPushButton* Button;

 public:
  explicit ServerCommandsWidget(QWidget* parent = nullptr);
  ~ServerCommandsWidget() = default;
  Q_DISABLE_COPY_MOVE(ServerCommandsWidget)

 private:
  void create(void);
  void connectDependencies(void);

 private slots:
  void prepareCommandExecution(void);

 signals:
  void executeCommand_signal(const QString& name);
};

#endif  // SERVERCOMMANDSWIDGET_H
