#ifndef ASYNCPRODUCTIONMANAGER_H
#define ASYNCPRODUCTIONMANAGER_H

#include "production_manager.h"
#include "progressable_async_wrapper.h"

class ProductionManagerAsyncWrapper : public ProgressableAsyncWrapper {
  Q_OBJECT

 private:
  std::unique_ptr<ProductionManager> Manager;

 public:
  Q_INVOKABLE explicit ProductionManagerAsyncWrapper(const QString& name);
  ~ProductionManagerAsyncWrapper() = default;

 public slots:
  void logOn(const StringDictionary& param);
  void logOut(void);

  void requestBox(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void rereleaseTransponder(const StringDictionary& param);
  void rollbackTransponder(void);

 private:
  Q_DISABLE_COPY_MOVE(ProductionManagerAsyncWrapper)
};

#endif  // ASYNCPRODUCTIONMANAGER_H
