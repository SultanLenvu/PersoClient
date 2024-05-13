#ifndef PROGRAMMERGUISUBKERNEL_H
#define PROGRAMMERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class ProgrammerGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  AbstractGui* CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

 public:
  explicit ProgrammerGuiSubkernel(const QString& name);
  ~ProgrammerGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void connectGui(AbstractGui* gui) override;
  virtual void connectManager(
      std::shared_ptr<AbstractManager> manager) override;
  virtual void reset() override;

 public slots:
  void displayUcid_slot(const std::shared_ptr<QString> ucid);

 private:
  Q_DISABLE_COPY_MOVE(ProgrammerGuiSubkernel);

  void connectAuthorizationGui();
  void connectMasterGui();
  void connectProductionAssemblerGui();
  void connectProductionTesterGui();

  void connectProgrammerManager(void) const;

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
