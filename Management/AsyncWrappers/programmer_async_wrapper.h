#ifndef PROGRAMMERASYNCWRAPPER_H
#define PROGRAMMERASYNCWRAPPER_H

#include "i_.h"
#include "progressable_async_wrapper.h"

class ProgrammerAsyncWrapper : public ProgressableAsyncWrapper {
 private:
  std::unique_ptr<IStickerPrinter> StickerPrinter;

 public:
  Q_INVOKABLE explicit ProgrammerAsyncWrapper(const QString& name);
  ~ProgrammerAsyncWrapper() = default;
};

#endif  // PROGRAMMERASYNCWRAPPER_H
