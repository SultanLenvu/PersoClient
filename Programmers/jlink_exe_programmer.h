#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include <QDir>
#include <QProcess>

#include "configurable_object.h"
#include "i_programmer.h"
#include "loggable_object.h"
#include "named_object.h"

class JLinkExeProgrammer : public NamedObject,
                           public IProgrammer,
                           public LoggableObject,
                           public ConfigurableObject {
  Q_OBJECT
 private:
  uint32_t Speed;
  QString JLinkPath;
  std::unique_ptr<QProcess> JLinkProcess;

  std::unique_ptr<QFile> JLinkScript;

  QStringList ProcessArguments;

 public:
  Q_INVOKABLE explicit JLinkExeProgrammer(const QString& name);
  ~JLinkExeProgrammer() = default;

 public:  // IProgrammer interface
  virtual ReturnStatus checkConfig(void) override;

  virtual ReturnStatus programMemory(const QByteArray& data) override;
  virtual ReturnStatus programMemory(const QString& fileName) override;
  virtual ReturnStatus programMemoryWithUnlock(const QByteArray& data) override;
  virtual ReturnStatus programMemoryWithUnlock(
      const QString& fileName) override;
  virtual ReturnStatus readMemory(QByteArray& data) override;
  virtual ReturnStatus readMemory(const QString& fileName) override;
  virtual ReturnStatus eraseMemory(void) override;

  virtual ReturnStatus programUserData(const QByteArray& data) override;
  virtual ReturnStatus programUserData(const QString& fileName) override;
  virtual ReturnStatus readUserData(QByteArray& data) override;
  virtual ReturnStatus readUserData(const QString& fileName) override;

  virtual ReturnStatus readTransponderUcid(QString& ucid) override;

  virtual ReturnStatus unlockMemory(void) override;
  virtual ReturnStatus lockMemory(void) override;

 private:
  Q_DISABLE_COPY_MOVE(JLinkExeProgrammer);
  virtual void loadSettings(void) override;
  void doLoadSettings(void);

 private:
  void createJLinkProcess(void);

  bool executeJLinkScript(void);
  bool executeJLinkScript(QString& output);
  void initScript(void);
};

#endif  // JLINKMANUALPROGRAMMER_H
