#ifndef PROGRAMMERGUISUBKERNEL_H
#define PROGRAMMERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class ProgrammerGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT

 public:
  explicit ProgrammerGuiSubkernel(const QString& name);
  ~ProgrammerGuiSubkernel() = default;

 private:
  Q_DISABLE_COPY_MOVE(ProgrammerGuiSubkernel);
  void connectDependencies(void) const;

 private:
  void programMemory(void);
  void readMemory(void);
  void eraseMemory(void);

  void programUserData(void);
  void readUserData(void);
  void readUcid(void);

  void unlockMemory(void);
  void lockMemory(void);

 signals:
  void programMemory_signal(const QByteArray& data);
  void readMemory_signal(void);
  void eraseMemory_signal(void);

  void programUserData_signal(const QByteArray& data);
  void readUserData_signal(void);
  void readUcid_signal(void);

  void unlockMemory_signal(void);
  void lockMemory_signal(void);
};
#endif  // PROGRAMMERGUISUBKERNEL_H
