#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>
#include <QThread>

#include "interface_programmer.h"

class JLinkExeProgrammer : public IProgrammer {
  Q_OBJECT
 private:
  QProcess *JLinkProcess;

  QFile *JLinkScript;

  QStringList ProcessArguments;
  QStringList ProcessOutput;

  QMutex Mutex;

 public:
  explicit JLinkExeProgrammer(QObject* parent);
  ~JLinkExeProgrammer();

 public slots:
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
  void processingJLinkExePath(const QString& path);
  void excuteJLinkScript(void);
  void initScript(void);
};

#endif // JLINKMANUALPROGRAMMER_H
