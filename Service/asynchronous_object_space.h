#ifndef ASYNCHRONOUSOBJECTSPACE_H
#define ASYNCHRONOUSOBJECTSPACE_H

#include <QThread>

#include "i_async_wrapper.h"

class AsynchronousObjectSpace final {
 private:
  QThread Thread;
  std::vector<std::unique_ptr<IAsyncWrapper>> Managers;

 public:
  explicit AsynchronousObjectSpace();
  ~AsynchronousObjectSpace();

 private:
  Q_DISABLE_COPY_MOVE(AsynchronousObjectSpace)

 private:
  void createWrappers(void);
};

#endif // ASYNCHRONOUSOBJECTSPACE_H