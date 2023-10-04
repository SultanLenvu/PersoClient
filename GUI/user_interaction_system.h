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

class UserInteractionSystem : public QObject {
  Q_OBJECT

 private:
  QWidget* ParentWindow;

  QProgressDialog* ProgressDialog;
  uint32_t CurrentOperationStep;

 public:
  explicit UserInteractionSystem(QObject* parent, QWidget* window);

 public slots:
  void generateMessage(const QString& pass);
  void generateErrorMessage(const QString& text);

  void getAuthorizationData(QMap<QString, QString>* data);
  void getMasterPassword(QString& pass);
  void getTransponderStickerData(QStringList* data, bool& ok);
  void getCustomTransponderStickerData(QMap<QString, QString>* data, bool& ok);

  void generateProgressDialog(void);
  void completeProgressDialog(void);
  void performeProgressDialogStep(void);

 private:
  void closeProgressDialog(void);

 private slots:
  void on_ProgressDialogCanceled_slot(void);

 signals:
  void abortCurrentOperation(void);
};

#endif // USER_INTERACTION_SYSTEM_H
