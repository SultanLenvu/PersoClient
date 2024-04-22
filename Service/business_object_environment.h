#ifndef BUSINESSOBJECTENVIRONMENT_H
#define BUSINESSOBJECTENVIRONMENT_H

#include <QHash>

#include "abstract_object_environment.h"

#include "production_manager_gui_subkernel.h"
#include "programmer_gui_subkernel.h"
#include "server_connection_gui_subkernel.h"
#include "sticker_printer_gui_subkernel.h"

#include "async_production_manager.h"
#include "async_programmer.h"
#include "async_server_connection.h"
#include "async_sticker_printer.h"
#include "production_unit_context.h"

class BusinessObjectEnvironment : public AbstractObjectEnvironment {
 private:
  std::unique_ptr<AsyncStickerPrinter> StickerPrinter;
  std::unique_ptr<AsyncProgrammer> Programmer;
  std::unique_ptr<AsyncServerConnection> ServerConnection;

  std::shared_ptr<ProductionUnitContext> Context;
  std::unique_ptr<AsyncProductionManager> Manager;

 public:
  explicit BusinessObjectEnvironment(const ProductionManagerGuiSubkernel* pmgs,
                                     const ProgrammerGuiSubkernel* pgs,
                                     const StickerPrinterGuiSubkernel* spgs,
                                     const ServerConnectionGuiSubkernel* scgs);
  ~BusinessObjectEnvironment() = default;
  Q_DISABLE_COPY_MOVE(BusinessObjectEnvironment)

 public:
  template <typename F,
            typename S,
            template <typename, typename>
            class ConnectionStrategy>
  void connect(const F* fobj);

 private:
  void createObjects(void);
};

#endif // BUSINESSOBJECTENVIRONMENT_H
