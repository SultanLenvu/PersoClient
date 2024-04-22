#ifndef CONNECTABLEOBJECT_H
#define CONNECTABLEOBJECT_H

#include <QObject>
#include <memory>

#include "i_connectable.h"

template <typename T>
class ConnectableObject : public IConnectable {
 private:
  std::unique_ptr<T> Object;

 public:
  template <typename... Args>
  explicit ConnectableObject(Args... args) {
    Object = std::make_unique<T>(std::forward<Args>(args)...);
  };
  virtual ~ConnectableObject() = default;

 public:
  virtual void connect(QObject* obj) override{

  };
  virtual void disconnect(QObject* obj) override{

  };
};

#endif // CONNECTABLEOBJECT_H
