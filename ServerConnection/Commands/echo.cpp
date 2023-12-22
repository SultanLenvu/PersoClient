#include "echo.h"

Echo::Echo(const QString& name) : AbstractClientCommand(name) {}

Echo::~Echo() {}

const QString& Echo::name() {
  return Name;
}

ReturnStatus Echo::generate(const StringDictionary& param,
                            QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды
  Request["data"] = "test";

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus Echo::processResponse(const QByteArray& dataBlock,
                                   StringDictionary& responseData) {
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
