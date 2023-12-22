#ifndef LOGIN_H
#define LOGIN_H

#include "abstract_client_command.h"
#include "definitions.h"

class LogInCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = COMMAND_ECHO_NAME;
  const size_t ResponseSize = COMMAND_ECHO_RESPONSE_SIZE;

 public:
  explicit LogInCommand(const QString& name);
  ~LogInCommand();

  // AbstractClientCommand interface
 public:
  virtual const QString& name() override;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) override;

 private:
  Q_DISABLE_COPY_MOVE(LogInCommand)

 signals:
};

#endif  // LOGIN_H
