#ifndef USERNOTIFICATION_H
#define USERNOTIFICATION_H

#include <QObject>

class UserNotification : public QObject
{
  Q_OBJECT
public:
  explicit UserNotification(QObject *parent = nullptr);

signals:

};

#endif // USERNOTIFICATION_H
