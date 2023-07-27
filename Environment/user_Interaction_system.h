#ifndef USER_INTERACTION_SYSTEM_H
#define USER_INTERACTION_SYSTEM_H

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>

#include "../Environment/programmer_manager.h"

class UserInteractionSystem : public QObject {
  Q_OBJECT

private:
  QWidget *ParentWindow;

public:
  explicit UserInteractionSystem(QObject *parent, QWidget *window);

public slots:
  void firmwareManagerNotification(ProgrammerManager::PerfomingStatus status);
  void getMasterPassword(QString &pass);
  void generateErrorMessage(const QString &text);
};

#endif // USER_INTERACTION_SYSTEM_H
