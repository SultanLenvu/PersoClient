#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QThread>

#include "interface_programmer.h"
#include "../Environment/definitions.h"

class JLinkExeProgrammer : public InterfaceProgrammer {

private:
  QFileInfo *JLinkExeInfo;
  QProcess *JLinkProcess;

  QFileInfo *JLinkScriptInfo;
  QFile *JLinkScript;

  QStringList ProcessArguments;
  QStringList ProcessOutput;

public:
  explicit JLinkExeProgrammer(QObject *parent = nullptr);
  ~JLinkExeProgrammer();

public slots:
  virtual void connectDevice(void) override;
  virtual void loadFirmware(void) override;
  virtual void loadUserData(void) override;
  virtual void readFirmware(void) override;
  virtual void readUserData(void) override;
  virtual void eraseFirmware(void) override;

  virtual void resetDevice(void) override;
  virtual void runDevice(void) override;
  virtual void unlockDevice(void) override;
  virtual void exit(void) override;

private:
  void processingJLinkExePath(const QString& path);
  void excuteJLinkScript(void);
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
