#ifndef ICONNECTABLE_H
#define ICONNECTABLE_H

#include <QObject>

class IConnectable
{
 public:
  explicit IConnectable() = default;
  virtual ~IConnectable() = default;
  Q_DISABLE_COPY_MOVE(IConnectable)

 public:
  virtual void connect(QObject* obj) = 0;
  virtual void disconnect(QObject* obj) = 0;
};

#endif // ICONNECTABLE_H
