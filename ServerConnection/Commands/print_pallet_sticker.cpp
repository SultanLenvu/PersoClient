#include "print_pallet_sticker.h"

PalletStickerPrintCommand::PalletStickerPrintCommand(const QString& name)
    : AbstractClientCommand(name) {}

PalletStickerPrintCommand::~PalletStickerPrintCommand() {}

const QString& PalletStickerPrintCommand::name() {
  return Name;
}

ReturnStatus PalletStickerPrintCommand::generate(const StringDictionary& param,
                                                 QByteArray& dataBlock) {
  // Заголовок команды
  Request["command_name"] = Name;

  // Тело команды
  Request["transponder_pan"] = param.value("transponder_pan");

  generateDataBlock(dataBlock);
  return ReturnStatus::NoError;
}

ReturnStatus PalletStickerPrintCommand::processResponse(
    const QByteArray& dataBlock,
    StringDictionary& responseData) {
  if (!processDataBlock(dataBlock)) {
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return ReturnStatus::ServerResponseDataBlockError;
  }

  if ((Response.size() != ResponseSize) || (Response["command_name"] != Name) ||
      (!Response.contains("return_status"))) {
    return ReturnStatus::ServerResponseSyntaxError;
  }

  ReturnStatus ret = processReturnStatus(Response["return_status"].toString());
  if (ret != ReturnStatus::NoError) {
    sendLog("Получена ошибка при выполнении команды на сервере.");
    return ret;
  }

  return ReturnStatus::NoError;
}
