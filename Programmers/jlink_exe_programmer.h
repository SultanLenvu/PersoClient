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
  QProcess *JLinkProcess;

  QFile *JLinkScript;

  QStringList ProcessArguments;
  QStringList ProcessOutput;

 public:
  explicit JLinkExeProgrammer(QObject* parent);
  ~JLinkExeProgrammer();

 public slots:
  virtual void getUcid(QString* ucid) override;
  virtual void loadFirmware(QFile* firmware) override;
  virtual void loadFirmwareWithUnlock(QFile* firmware) override;
  virtual void readFirmware(void) override;
  virtual void eraseFirmware(void) override;

  virtual void readData(void) override;
  virtual void loadData(QFile* data) override;

  virtual void unlockDevice(void) override;
  virtual void lockDevice(void) override;

  virtual void applySettings() override;

 private:
  void loadSettings(void);
  void excuteJLinkScript(void);
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
