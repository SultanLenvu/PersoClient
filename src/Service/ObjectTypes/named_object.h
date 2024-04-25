#ifndef NAMED_OBJECT_H
#define NAMED_OBJECT_H

#include <QObject>

class NamedObject : public QObject {
  Q_OBJECT

 public:
  explicit NamedObject(const QString& name);
  virtual ~NamedObject() = default;

 private:
  NamedObject();
  Q_DISABLE_COPY_MOVE(NamedObject)
};

#endif  // NAMED_OBJECT_H
