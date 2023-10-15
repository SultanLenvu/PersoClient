#ifndef USER_INTERACTION_SYSTEM_H
#define USER_INTERACTION_SYSTEM_H

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QProgressBar>
#include <QProgressDialog>

#include "authorization_dialog.h"
#include "custom_transponder_sticker_dialog.h"
#include "transponder_sticker_scan_dialog.h"

class UserInteractionSystem : public QWidget {
  Q_OBJECT

 private:
  bool LogEnable;
  QProgressDialog* ProgressDialog;
  uint32_t CurrentOperationStep;

  QTimer* ODTimer;
  QTimer* ODQTimer;
  QElapsedTimer* ODMeter;

 public:
  explicit UserInteractionSystem(QWidget* window);

 public slots:
  void generateMessage(const QString& pass);
  void generateErrorMessage(const QString& text);

  void startOperationProgressDialog(const QString& operationName);
  void finishOperationProgressDialog(const QString& operationName);

  void getMasterPassword(QString& pass);
  void getAuthorizationData(QMap<QString, QString>* data);
  void getTransponderStickerData(QStringList* data, bool& ok);
  void getCustomTransponderStickerData(QMap<QString, QString>* data, bool& ok);

  void applySettings(void);

 private:
  Q_DISABLE_COPY(UserInteractionSystem)
  void loadSettings(void);
  void sendLog(const QString& log);

  void createProgressDialog(void);
  void destroyProgressDialog(void);
  void createTimers(void);

 private slots:
  void on_ODTimerTimeout_slot(void);
  void on_ODQTimerTimeout_slot(void);

 signals:
  void logging(const QString& log);
  void abortCurrentOperation(void);
};

#endif  // USER_INTERACTION_SYSTEM_H
