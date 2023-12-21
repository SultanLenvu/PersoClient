#include "log_in_command.h"

LogInCommand::LogInCommand(const QString& name) : AbstractClientCommand(name) {}

LogInCommand::~LogInCommand() {}

const QString& LogInCommand::name() {
  return Name;
}

ReturnStatus LogInCommand::generate(QByteArray& dataBlock) {
  return ReturnStatus::ClientCommandParamError;
}

ReturnStatus LogInCommand::generate(const StringDictionary& param,
                                    QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды
  Request["login"] = param.value("login");
  Request["password"] = param.value("password");

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus LogInCommand::processResponse(const QByteArray& dataBlock) {
  return ReturnStatus::ServerResponseSyntaxError;
}

ReturnStatus LogInCommand::processResponse(const QByteArray& dataBlock,
                                           StringDictionary& responseData) {
  if (!processDataBlock(dataBlock)) {
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return ReturnStatus::ServerResponseDataBlockError;
  }

  if ((Response.size() != Size) || (Response["command_name"] != Name) ||
      (!Response.contains("return_status"))) {
    return ReturnStatus::ServerResponseSyntaxError;
  }

  responseData.insert("return_status", Response["return_status"].toString());

  return ReturnStatus::NoError;
}

void LogInCommand::clear() {
  Request = QJsonObject();
  Response = QJsonObject();
}
