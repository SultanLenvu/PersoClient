#include "get_transponder_data.h"

GetTransponderData::GetTransponderData(const QString& name)
    : AbstractClientCommand(name) {}

GetTransponderData::~GetTransponderData() {}

const QString& GetTransponderData::name() {
  return Name;
}

ReturnStatus GetTransponderData::generate(const StringDictionary& param,
                                          QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды
  Request["transponder_pan"] = param.value("transponder_pan");

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus GetTransponderData::processResponse(
    const QByteArray& dataBlock,
    StringDictionary& responseData) {
  if (!processDataBlock(dataBlock)) {
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return ReturnStatus::ServerResponseDataBlockError;
  }

  if (!checkSyntax()) {
    return ReturnStatus::ServerResponseSyntaxError;
  }

  responseData["transponder_sn"] = Response["transponder_sn"].toString();
  responseData["transponder_pan"] = Response["transponder_pan"].toString();
  responseData["box_id"] = Response["box_id"].toString();
  responseData["pallet_id"] = Response["pallet_id"].toString();
  responseData["order_id"] = Response["order_id"].toString();
  responseData["issuer_name"] = Response["issuer_name"].toString();
  responseData["transponder_model"] = Response["transponder_model"].toString();

  ReturnStatus ret = processReturnStatus(Response["return_status"].toString());
  if (ret != ReturnStatus::NoError) {
    sendLog("Получена ошибка при выполнении команды на сервере.");
    return ret;
  }

  return ReturnStatus::NoError;
}

bool GetTransponderData::checkSyntax() {
  if ((Response.size() != ResponseSize) || (Response["command_name"] != Name) ||
      !Response.contains("return_status") ||
      !Response.contains("transponder_sn") ||
      !Response.contains("transponder_pan") || !Response.contains("box_id") ||
      !Response.contains("pallet_id") || !Response.contains("order_id") ||
      !Response.contains("issuer_name") ||
      !Response.contains("transponder_model")) {
    return false;
  }

  return true;
}
