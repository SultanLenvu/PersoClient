#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QtClassHelperMacros>

class AbstractManager {
 public:
  explicit AbstractManager();
  virtual ~AbstractManager();

 private:
  Q_DISABLE_COPY_MOVE(AbstractManager);
};

#endif  // ABSTRACTMANAGER_H
