#ifndef PROGRAMMERGUISUBKERNEL_H
#define PROGRAMMERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"
#include "programmer_manager.h"

class ProgrammerGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  std::shared_ptr<AbstractGui> CurrentGui;

 public:
  explicit ProgrammerGuiSubkernel(const QString& name);
  ~ProgrammerGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectAuthorizationGui(
      std::shared_ptr<AuthorizationGui> gui) override;
  virtual void connectMasterGui(std::shared_ptr<MasterGui> gui) override;
  virtual void connectProductionAssemblerGui(
      std::shared_ptr<ProductionAssemblerGui> gui) override;
  virtual void connectProductionTesterGui(std::shared_ptr<ProductionTesterGui> gui) override;
  virtual void resetCurrentGui() override;

 public:
  void connectManager(const ProgrammerManager* manager) const;

 public slots:
  void displayUcid_slot(const std::shared_ptr<QString> ucid);

 private:
  Q_DISABLE_COPY_MOVE(ProgrammerGuiSubkernel);

  void programMemory_guiSlot(void);
  void readMemory_guiSlot(void);
  void eraseMemory_guiSlot(void);

  void programUserData_guiSlot(void);
  void readUserData_guiSlot(void);
  void readUcid_guiSlot(void);

  void unlockMemory_guiSlot(void);
  void lockMemory_guiSlot(void);

 signals:
  void programMemory_signal(const std::shared_ptr<QString> path);
  void readMemory_signal(void);
  void eraseMemory_signal(void);

  void programUserData_signal(const std::shared_ptr<QString> path);
  void readUserData_signal(void);
  void readUcid_signal(void);

  void unlockMemory_signal(void);
  void lockMemory_signal(void);
};
#endif  // PROGRAMMERGUISUBKERNEL_H
