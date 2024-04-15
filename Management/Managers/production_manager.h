#ifndef PRODUCTIONUNITMANAGER_H
#define PRODUCTIONUNITMANAGER_H

#include <QFile>

#include "abstract_manager.h"
#include "i_programmer.h"
#include "i_server_connection.h"
#include "i_sticker_printer.h"
#include "loggable_object.h"
#include "named_object.h"
#include "production_unit_context.h"

class ProductionManager : public NamedObject,
                          public AbstractManager,
                          public LoggableObject {
  Q_OBJECT

 private:
  std::shared_ptr<ProductionUnitContext> Context;

  std::shared_ptr<IServerConnection> Server;
  std::shared_ptr<IProgrammer> Programmer;
  std::shared_ptr<IStickerPrinter> StickerPrinter;

 public:
  explicit ProductionManager(const QString& name,
                             std::shared_ptr<ProductionUnitContext> context,
                             std::shared_ptr<IServerConnection> server,
                             std::shared_ptr<IStickerPrinter> stickerPrinter,
                             std::shared_ptr<IProgrammer> programmer);
  ~ProductionManager() = default;

 public:
  ReturnStatus logOn(const StringDictionary& param);
  ReturnStatus logOut(void);

  ReturnStatus requestBox(void);
  ReturnStatus refundCurrentBox(void);
  ReturnStatus completeCurrentBox(void);

  ReturnStatus releaseTransponder(void);
  ReturnStatus rereleaseTransponder(const StringDictionary& param);
  ReturnStatus rollbackTransponder(void);

 private:
  Q_DISABLE_COPY_MOVE(ProductionManager)
};

#endif  // PRODUCTIONUNITMANAGER_H
