#include "echo_comand.h"

EchoCommand::EchoCommand(const QString& name) : AbstractClientCommand(name) {}

EchoCommand::~EchoCommand() {}

const QString& EchoCommand::name() {
  return Name;
}

ReturnStatus EchoCommand::generate(QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды
  Request["data"] = "test";

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus EchoCommand::generate(const StringDictionary& param,
                                   QByteArray& dataBlock) {
  return ReturnStatus::ClientCommandParamError;
}

ReturnStatus EchoCommand::processResponse(const QByteArray& dataBlock) {
  if (!processDataBlock(dataBlock)) {
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return ReturnStatus::ServerResponseDataBlockError;
  }

  if ((Response.size() != Size) || (Response["command_name"] != Name) ||
      (!Response.contains("data"))) {
    return ReturnStatus::ServerResponseSyntaxError;
  }

  if (Request["data"] != Response["data"]) {
    sendLog("Получена ошибка при обработке ответа.");
    return ReturnStatus::ServerResponseProcessingError;
  }

  return ReturnStatus::NoError;
}

ReturnStatus EchoCommand::processResponse(const QByteArray& dataBlock,
                                          StringDictionary& responseData) {
  return ReturnStatus::ServerResponseSyntaxError;
}

void EchoCommand::clear() {
  Request = QJsonObject();
  Response = QJsonObject();
}
