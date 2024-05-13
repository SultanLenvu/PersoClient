#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QtClassHelperMacros>

class AbstractManager {
 public:
  explicit AbstractManager() = default;
  virtual ~AbstractManager() = default;

 private:
  Q_DISABLE_COPY_MOVE(AbstractManager);
};

#endif  // ABSTRACTMANAGER_H
