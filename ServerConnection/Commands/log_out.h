#ifndef LOGOUT_H
#define LOGOUT_H

#include "abstract_client_command.h"
#include "definitions.h"

class LogOutCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = COMMAND_LOGOUT_NAME;
  const size_t ResponseSize = COMMAND_LOGOUT_RESPONSE_SIZE;

 public:
  explicit LogOutCommand(const QString& name);
  ~LogOutCommand();

  // AbstractClientCommand interface
 public:
  virtual const QString& name() override;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) override;

 private:
  Q_DISABLE_COPY_MOVE(LogOutCommand)

 signals:
};

#endif  // LOGOUT_H
