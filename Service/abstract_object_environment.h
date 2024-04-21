#ifndef ABSTRACTOBJECTENVIRONMENT_H
#define ABSTRACTOBJECTENVIRONMENT_H

#include <QObject>

class AbstractObjectEnvironment : public QObject {
 public:
  explicit AbstractObjectEnvironment() = default;
  virtual ~AbstractObjectEnvironment() = default;
  Q_DISABLE_COPY_MOVE(AbstractObjectEnvironment)
};

#endif  // ABSTRACTOBJECTENVIRONMENT_H
