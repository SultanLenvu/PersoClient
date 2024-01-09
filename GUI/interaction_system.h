#ifndef INTERACTION_SYSTEM_H
#define INTERACTION_SYSTEM_H

#include <QElapsedTimer>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QProgressBar>
#include <QProgressDialog>
#include <QTimer>

#include "types.h"

class InteractionSystem : public QObject {
  Q_OBJECT

 private:
  std::unique_ptr<QProgressDialog> ProgressDialog;

  std::unique_ptr<QTimer> ODTimer;
  std::unique_ptr<QTimer> ODQTimer;
  std::unique_ptr<QElapsedTimer> ODMeter;

 public:
  explicit InteractionSystem(const QString& name);

 public slots:
  void generateMessage(const QString& data);
  void generateErrorMessage(const QString& text);

  void operationStarted(const QString& opName);
  void operationFinished(const QString& opName, ReturnStatus ret);

  void applySettings(void);

 private:
  Q_DISABLE_COPY_MOVE(InteractionSystem)
  void sendLog(const QString& log);
  void loadSettings(void);

  void createProgressDialog(void);
  void destroyProgressDialog(void);
  void createTimers(void);

 private slots:
  void odTimerTimeout_slot(void);
  void odqTimerTimeout_slot(void);

 signals:
  void abortCurrentOperation(void);
  void logging(const QString& log);
};

#endif  // INTERACTION_SYSTEM_H
