#ifndef ASYNCPRODUCTIONMANAGER_H
#define ASYNCPRODUCTIONMANAGER_H

#include "loggable_object.h"
#include "production_manager.h"
#include "progressable_async_object.h"

class AsyncProductionManager final : public AbstractAsyncObject,
                                     public LoggableObject {
  Q_OBJECT

 private:
  std::unique_ptr<ProductionManager> Manager;

 public:
  explicit AsyncProductionManager(std::unique_ptr<ProductionManager> manager);
  ~AsyncProductionManager() = default;
  Q_DISABLE_COPY_MOVE(AsyncProductionManager)

 public slots:
  void logOn(const StringDictionary& param);
  void logOut(void);

  void requestBox(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void rereleaseTransponder(const StringDictionary& param);
  void rollbackTransponder(void);
};

#endif  // ASYNCPRODUCTIONMANAGER_H
