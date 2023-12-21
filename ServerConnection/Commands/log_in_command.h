#ifndef LogInCommand_H
#define LogInCommand_H

#include "abstract_client_command.h"
#include "definitions.h"

class LogInCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = "log_in";
  const size_t Size = COMMAND_ECHO_SIZE;

 public:
  explicit LogInCommand(const QString& name);
  ~LogInCommand();

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
  Q_DISABLE_COPY_MOVE(LogInCommand)

 signals:
};

#endif  // LogInCommand_H
