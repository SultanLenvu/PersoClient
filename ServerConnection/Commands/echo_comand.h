#ifndef ECHOCOMMAND_H
#define ECHOCOMMAND_H

#include "abstract_client_command.h"

class EchoCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = "echo";
  const size_t Size = 2;

 public:
  explicit EchoCommand(const QString& name);
  ~EchoCommand();

  // AbstractClientCommand interface
 public:
  virtual const QString& name() override;
  virtual ReturnStatus generate(QByteArray& dataBlock) override;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) override;
  virtual void clear() override;

 private:
  Q_DISABLE_COPY_MOVE(EchoCommand)
};

#endif  // ECHOCOMMAND_H
