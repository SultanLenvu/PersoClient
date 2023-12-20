#ifndef PROGRAMMERMANAGER_H
#define PROGRAMMERMANAGER_H

#include "abstract_manager.h"

class ProgrammerManager : public AbstractManager {
  Q_OBJECT
 public:
  explicit ProgrammerManager(const QString& name);
  ~ProgrammerManager();

 public slots:
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

 signals:
  void displayUcid(const std::shared_ptr<QString> ucid);
};

#endif  // PROGRAMMERMANAGER_H
