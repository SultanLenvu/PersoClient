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
 virtual void loadFirmware(const QString& firmwareFileName) override;
 virtual void loadFirmwareWithUnlock(const QString& firmwareFileName) override;
 virtual void readFirmware(void) override;
 virtual void eraseFirmware(void) override;

 virtual void readUserData(void) override;
 virtual void loadUserData(const QString& userDataFileName) override;

 virtual void unlockDevice(void) override;
 virtual void lockDevice(void) override;

 virtual void exit(void) override;
 virtual void applySettings() override;

private:
 void processingJLinkExePath(const QString& path);
 void excuteJLinkScript(void);
 void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
