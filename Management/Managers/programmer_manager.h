#ifndef PROGRAMMERMANAGER_H
#define PROGRAMMERMANAGER_H

#include "abstract_manager.h"
#include "i_programmer.h"

class ProgrammerManager : public AbstractManager {
  Q_OBJECT

 private:
  std::unique_ptr<IProgrammer> Programmer;

 public:
  explicit ProgrammerManager(const QString& name);
  ~ProgrammerManager();

  // AbstractManager interface
 public:
  virtual void onInstanceThreadStarted(void) override;
  virtual Type type() const override;

 public slots:
  virtual void applySettings(void) override;

  void programMemory(const std::shared_ptr<QString> path);
  void readMemory(void);
  void eraseMemory(void);

  void programUserData(const std::shared_ptr<QString> path);
  void readUserData(void);
  void readUcid(void);

  void unlockMemory(void);
  void lockMemory(void);

 private:
  Q_DISABLE_COPY_MOVE(ProgrammerManager)
  void sendLog(const QString& log);
  void loadSettings(void);

  void createProgrammer(void);

 signals:
  void displayUcid_signal(const std::shared_ptr<QString> ucid);
};

#endif  // PROGRAMMERMANAGER_H
