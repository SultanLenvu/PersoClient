#include "async_object_space.h"

AsyncObjectSpace::AsyncObjectSpace() {
  Thread.start();
  Factory = std::make_unique<QObjectFactory>(&Thread);
}

AsyncObjectSpace::~AsyncObjectSpace() {
  Thread.quit();
  Thread.wait();
}
