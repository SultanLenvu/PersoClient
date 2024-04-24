#ifndef USERINTERFACEBUILDER_H
#define USERINTERFACEBUILDER_H

#include "i_user_interface_factory.h"

class UserInterfaceFactory final : public IUserInterfaceFactory {
 public:
  explicit UserInterfaceFactory() = default;
  ~UserInterfaceFactory() = default;
  Q_DISABLE_COPY_MOVE(UserInterfaceFactory);

  // IUserInterfaceFactory interface
 public:
  virtual QWidget* createMasterUI(
      const ProductionManagerGuiSubkernel* pmgs,
      const ProgrammerGuiSubkernel* pgs,
      const StickerPrinterGuiSubkernel* spgs,
      const ServerConnectionGuiSubkernel* scgs) const override;
  virtual QWidget* createAssemblerUI(
      const ProductionManagerGuiSubkernel* pmgs) const override;
  virtual QWidget* createTesterUI(
      const ProductionManagerGuiSubkernel* pmgs) const override;
  virtual QWidget* createAuthorizationUI(
      const ProductionManagerGuiSubkernel* pmgs) const override;

 private:
  QWidget* createProductionManager(const ProductionManagerGuiSubkernel* pmgs);
  QWidget* createProgrammer(const ProgrammerGuiSubkernel* pmgs);
  QWidget* createServerCommands(const ServerConnectionGuiSubkernel* pmgs);
  QWidget* createStickerPrinter(const StickerPrinterGuiSubkernel* pmgs);
  QWidget* createLogDisplay();
};

#endif // USERINTERFACEBUILDER_H
