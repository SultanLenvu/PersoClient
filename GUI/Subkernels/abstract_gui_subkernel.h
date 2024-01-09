#ifndef ABSTRACTGUISUBKERNEL_H
#define ABSTRACTGUISUBKERNEL_H

#include <QObject>

#include "authorization_gui.h"
#include "master_gui.h"
#include "production_assembler_gui.h"
#include "production_tester_gui.h"

class AbstractGuiSubkernel : public QObject {
  Q_OBJECT
 public:
  explicit AbstractGuiSubkernel(const QString& name);
  virtual ~AbstractGuiSubkernel();

  virtual void connectAuthorizationGui(
      std::shared_ptr<AuthorizationGui> gui) = 0;
  virtual void connectMasterGui(std::shared_ptr<MasterGui> gui) = 0;
  virtual void connectProductionAssemblerGui(std::shared_ptr<ProductionAssemblerGui> gui) = 0;
  virtual void connectProductionTesterGui(std::shared_ptr<ProductionTesterGui> gui) = 0;
  virtual void resetCurrentGui(void);

 private:
  AbstractGuiSubkernel();
  Q_DISABLE_COPY_MOVE(AbstractGuiSubkernel);

 signals:
  void loggerClear_signal(void);
};

#endif  // ABSTRACTGUISUBKERNEL_H
