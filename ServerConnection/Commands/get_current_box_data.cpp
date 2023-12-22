#include "get_current_box_data.h"

GetCurrentBoxData::GetCurrentBoxData(const QString& name)
    : AbstractClientCommand(name) {}

GetCurrentBoxData::~GetCurrentBoxData() {}

const QString& GetCurrentBoxData::name() {
  return Name;
}

ReturnStatus GetCurrentBoxData::generate(const StringDictionary& param,
                                         QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus GetCurrentBoxData::processResponse(
    const QByteArray& dataBlock,
    StringDictionary& responseData) {
  if (!processDataBlock(dataBlock)) {
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return ReturnStatus::ServerResponseDataBlockError;
  }

  if (!checkSyntax()) {
    return ReturnStatus::ServerResponseSyntaxError;
  }

  responseData["quantity"] = Response["quantity"].toString();
  responseData["in_process"] = Response["in_process"].toString();
  responseData["id"] = Response["id"].toString();
  responseData["pallet_id"] = Response["pallet_id"].toString();
  responseData["production_line_id"] =
      Response["production_line_id"].toString();
  responseData["transponder_id"] = Response["transponder_id"].toString();
  responseData["assembled_units"] = Response["assembled_units"].toString();
  responseData["assembling_start"] = Response["assembling_start"].toString();
  responseData["assembling_end"] = Response["assembling_end"].toString();

  ReturnStatus ret = processReturnStatus(Response["return_status"].toString());
  if (ret != ReturnStatus::NoError) {
    sendLog("Получена ошибка при выполнении команды на сервере.");
    return ret;
  }

  return ReturnStatus::NoError;
}

bool GetCurrentBoxData::checkSyntax() {
  if ((Response.size() != ResponseSize) || (Response["command_name"] != Name) ||
      !Response.contains("return_status") || !Response.contains("quantity") ||
      !Response.contains("in_process") || !Response.contains("id") ||
      !Response.contains("pallet_id") ||
      !Response.contains("production_line_id") ||
      !Response.contains("transponder_id") ||
      !Response.contains("assembled_units") ||
      !Response.contains("assembling_start") ||
      !Response.contains("assembling_end")) {
    return false;
  }

  return true;
}
