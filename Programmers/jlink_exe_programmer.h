#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QThread>

#include "abstract_programmer.h"

class JLinkExeProgrammer : public AbstractProgrammer {
  Q_OBJECT
 private:
  uint32_t Speed;
  QString JLinkPath;
  std::unique_ptr<QProcess> JLinkProcess;

  std::unique_ptr<QFile> JLinkScript;

  QStringList ProcessArguments;
  QStringList ProcessOutput;

 public:
  explicit JLinkExeProgrammer(const QString& name);
  ~JLinkExeProgrammer();

 public:
  virtual ReturnStatus checkConfig(void) override;
  virtual ProgrammerType type() const override;
  virtual ReturnStatus programMemory(QFile& firmware) override;
  virtual ReturnStatus programMemoryWithUnlock(QFile& firmware) override;
  virtual ReturnStatus readMemory(void) override;
  virtual ReturnStatus eraseMemory(void) override;

  virtual ReturnStatus readUserData(void) override;
  virtual ReturnStatus programUserData(QFile& data) override;

  virtual ReturnStatus readUcid(QString& ucid) override;

  virtual ReturnStatus unlockMemory(void) override;
  virtual ReturnStatus lockMemory(void) override;

  virtual void applySettings() override;

 private:
  Q_DISABLE_COPY(JLinkExeProgrammer);
  void sendLog(const QString& log);
  void loadSettings(void);

  void createJLinkProcess(void);

  void executeJLinkScript(void);
  void initScript(void);
};

#endif  // JLINKMANUALPROGRAMMER_H
