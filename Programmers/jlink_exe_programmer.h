#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QThread>

#include "interface_programmer.h"

class JLinkExeProgrammer : public IProgrammer {
  Q_OBJECT
 private:
  bool LogEnable;
  bool ExtendedLoggingEnable;
  uint32_t Speed;
  QProcess *JLinkProcess;

  QFile *JLinkScript;

  QStringList ProcessArguments;
  QStringList ProcessOutput;

 public:
  explicit JLinkExeProgrammer(QObject* parent);
  ~JLinkExeProgrammer();

 public:
  virtual ReturnStatus getUcid(QString* ucid) override;
  virtual ReturnStatus loadFirmware(QFile* firmware) override;
  virtual ReturnStatus loadFirmwareWithUnlock(QFile* firmware) override;
  virtual ReturnStatus readFirmware(void) override;
  virtual ReturnStatus eraseFirmware(void) override;

  virtual ReturnStatus readData(void) override;
  virtual ReturnStatus loadData(QFile* data) override;

  virtual ReturnStatus unlockDevice(void) override;
  virtual ReturnStatus lockDevice(void) override;

  virtual void applySettings() override;

 private:
  void sendLog(const QString& log);
  void loadSettings(void);
  void excuteJLinkScript(void);
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
