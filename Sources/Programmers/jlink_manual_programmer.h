#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QThread>

#include "interface_programmer.h"
#include "../Environment/definitions.h"

class JLinkManualProgrammer : public InterfaceProgrammer {

private:
  QFileInfo *JLinkExeInfo;
  QProcess *JLinkProcess;

  QFileInfo *JLinkScriptInfo;
  QFile *JLinkScript;

  QStringList ProcessArguments;
  QByteArray ProcessOutput;

public:
  explicit JLinkManualProgrammer(QObject *parent = nullptr);
  ~JLinkManualProgrammer();

public slots:
  virtual void connect(void) override;
  virtual void erase(void) override;
  virtual void load(QFile *firmware) override;

private:
  void processingJLinkExePath(const QString& path);
  void excuteJLinkScript(void);
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
