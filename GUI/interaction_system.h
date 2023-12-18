#ifndef INTERACTION_SYSTEM_H
#define INTERACTION_SYSTEM_H

#include <QElapsedTimer>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QProgressBar>
#include <QProgressDialog>

/*!
 * Interaction system singleton. Show dialogs, messages, etc
 */
class InteractionSystem : public QWidget {
  Q_OBJECT

 private:
  QProgressDialog* ProgressDialog;
  uint32_t CurrentOperationStep;

  QTimer* ODTimer;
  QTimer* ODQTimer;
  QElapsedTimer* ODMeter;

 public:
  static InteractionSystem* instance(void);

 public slots:
  void generateMessage(const QString& data);
  void generateErrorMessage(const QString& text);

  void startOperationProgressDialog(const QString& operationName);
  void finishOperationProgressDialog(const QString& operationName);

  bool getMasterPassword(QString& pass);
  bool getAuthorizationData(QHash<QString, QString>* data);
  bool getTransponderStickerData(QHash<QString, QString>* data);
  bool getCustomTransponderStickerData(QHash<QString, QString>* data);

  void applySettings(void);

 private:
  explicit InteractionSystem(QWidget* window);
  Q_DISABLE_COPY(InteractionSystem)

  void loadSettings(void);
  void createProgressDialog(void);
  void destroyProgressDialog(void);
  void createTimers(void);

 private slots:
  void on_ODTimerTimeout_slot(void);
  void on_ODQTimerTimeout_slot(void);

 signals:
  void abortCurrentOperation(void);
};

#endif  // INTERACTION_SYSTEM_H
