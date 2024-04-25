#ifndef IUSERINTERFACEFACTORY_H
#define IUSERINTERFACEFACTORY_H

#include <QWidget>

#include "production_manager_gui_subkernel.h"
#include "programmer_gui_subkernel.h"
#include "server_connection_gui_subkernel.h"
#include "sticker_printer_gui_subkernel.h"

class IUserInterfaceFactory
{
 public:
  explicit IUserInterfaceFactory() = default;
  ~IUserInterfaceFactory() = default;
  Q_DISABLE_COPY_MOVE(IUserInterfaceFactory);

 public:
  virtual QWidget* createMasterUI(
      const ProductionManagerGuiSubkernel* pmgs,
      const ProgrammerGuiSubkernel* pgs,
      const StickerPrinterGuiSubkernel* spgs,
      const ServerConnectionGuiSubkernel* scgs) const = 0;
  virtual QWidget* createAssemblerUI(
      const ProductionManagerGuiSubkernel* pmgs) const = 0;
  virtual QWidget* createTesterUI(
      const ProductionManagerGuiSubkernel* pmgs) const = 0;
  virtual QWidget* createAuthorizationUI(
      const ProductionManagerGuiSubkernel* pmgs) const = 0;
};

#endif // IUSERINTERFACEFACTORY_H
