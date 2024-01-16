#include "abstract_client_command.h"

#include <QDataStream>
#include <QIODevice>
#include <QJsonDocument>

#include "global_environment.h"
#include "log_system.h"

AbstractClientCommand::AbstractClientCommand(const QString& name)
    : QObject{nullptr} {
  setObjectName(name);

  connect(this, &AbstractClientCommand::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}

AbstractClientCommand::~AbstractClientCommand() {}

void AbstractClientCommand::clear() {
  //  Request = QJsonObject();
  //  Response = QJsonObject();
}

AbstractClientCommand::AbstractClientCommand() {}

void AbstractClientCommand::sendLog(const QString& log) {
  emit logging("Command " + objectName() + " - " + log);
}

void AbstractClientCommand::generateDataBlock(QByteArray& dataBlock) {
  QJsonDocument requestDocument(Request);

  // Инициализируем блок данных и сериализатор
  dataBlock.clear();
  QDataStream serializator(&dataBlock, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_6_5);

  // Формируем единый блок данных для отправки
  serializator << uint32_t(0) << requestDocument.toJson();
  serializator.device()->seek(0);
  serializator << uint32_t(dataBlock.size() - sizeof(uint32_t));

  sendLog(QString("Размер отправляемого блока данных: %1")
              .arg(QString::number(dataBlock.size())));
  sendLog(QString("Отправляемый блок данных: %1").arg(dataBlock));
}

bool AbstractClientCommand::processDataBlock(const QByteArray& dataBlock) {
  QJsonParseError status;
  QJsonDocument responseDocument = QJsonDocument::fromJson(dataBlock, &status);

  // Если пришел некорректный JSON
  if (status.error != QJsonParseError::NoError) {
    return false;
  }

  // Выделяем список пар ключ-значение из JSON-файла
  Response = responseDocument.object();
  return true;
}

ReturnStatus AbstractClientCommand::processReturnStatus(const QString& ret) {
  CommandReturnStatus cret = static_cast<CommandReturnStatus>(ret.toInt());

  sendLog(CrtLogMap.at(cret));

  return CrtMap.at(cret);
}

void AbstractClientCommand::createCrtMap() {
  CrtMap[NoError] = ReturnStatus::NoError;
  CrtMap[ParameterError] = ReturnStatus::ServerInternalError;
  CrtMap[SyntaxError] = ReturnStatus::ServerInternalError;
  CrtMap[SynchronizationError] = ReturnStatus::ServerInternalError;
  CrtMap[FileOpenError] = ReturnStatus::ServerInternalError;
  CrtMap[InvalidProductionLineContext] = ReturnStatus::ServerInternalError;
  CrtMap[UnauthorizedRequest] = ReturnStatus::ServerInternalError;
  CrtMap[DatabaseConnectionError] = ReturnStatus::ServerInternalError;
  CrtMap[DatabaseTransactionError] = ReturnStatus::ServerInternalError;
  CrtMap[FirmwareGeneratorInitError] = ReturnStatus::ServerInternalError;
  CrtMap[StickerPrinterInitError] = ReturnStatus::ServerInternalError;
  CrtMap[RecordMissed] = ReturnStatus::ServerInternalError;
  CrtMap[ProductionLineMissed] = ReturnStatus::ProductionLineMissed;
  CrtMap[TranspoderMissed] = ReturnStatus::ServerInternalError;
  CrtMap[BoxMissed] = ReturnStatus::ServerInternalError;
  CrtMap[PalletMissed] = ReturnStatus::ServerInternalError;
  CrtMap[OrderMissed] = ReturnStatus::ServerInternalError;
  CrtMap[MasterKeysMissed] = ReturnStatus::ServerInternalError;
  CrtMap[TransponderNotReleasedEarlier] = ReturnStatus::ServerInternalError;
  CrtMap[TransponderNotAwaitingConfirmation] =
      ReturnStatus::ServerInternalError;
  CrtMap[TransponderIncorrectRerelease] =
      ReturnStatus::TransponderIncorrectRerelease;
  CrtMap[IssuerMissed] = ReturnStatus::ServerInternalError;
  CrtMap[IdenticalUcidError] = ReturnStatus::IdenticalUcidError;
  CrtMap[CurrentOrderAssembled] = ReturnStatus::CurrentOrderAssembled;
  CrtMap[ProductionLineLaunchError] = ReturnStatus::ProductionLineLaunchError;
  CrtMap[ProductionLineAlreadyLaunched] =
      ReturnStatus::ProductionLineAlreadyLaunched;
  CrtMap[ProductionLineNotActive] = ReturnStatus::ProductionLineNotActive;
  CrtMap[ProductionLineNotInProcess] = ReturnStatus::ProductionLineNotInProcess;
  CrtMap[ProductionLineCompleted] = ReturnStatus::ProductionLineCompleted;
  CrtMap[ProductionLineNotInProcess] = ReturnStatus::ProductionLineNotInProcess;
  CrtMap[ProductionLineShutdownError] = ReturnStatus::ServerInternalError;
  CrtMap[ProductionLineRollbackLimit] =
      ReturnStatus::ProductionLineRollbackLimit;
  CrtMap[OrderInProcessMissed] = ReturnStatus::OrderInProcessMissed;
  CrtMap[FreeBoxMissed] = ReturnStatus::FreeBoxMissed;
  CrtMap[FirmwareGenerationError] = ReturnStatus::ServerInternalError;
  CrtMap[PrinterConnectionError] = ReturnStatus::ServerInternalError;
  CrtMap[PrinterLibraryError] = ReturnStatus::ServerInternalError;
  CrtMap[BoxStickerPrintError] = ReturnStatus::ServerInternalError;
  CrtMap[BoxStickerPrintError] = ReturnStatus::ServerInternalError;
  CrtMap[PalletStickerPrintError] = ReturnStatus::ServerInternalError;
  CrtMap[Unknown] = ReturnStatus::ServerInternalError;
}

void AbstractClientCommand::createCrtLogMap() {
  CrtLogMap[NoError] = "Server return status: NoError. ";
  CrtLogMap[ParameterError] = "Server return status: ParameterError";
  CrtLogMap[SyntaxError] = "Server return status: SyntaxError";
  CrtLogMap[SynchronizationError] =
      "Server return status: SynchronizationError";
  CrtLogMap[FileOpenError] = "Server return status: FileOpenError";
  CrtLogMap[InvalidProductionLineContext] =
      "Server return status: InvalidProductionLineContext";
  CrtLogMap[UnauthorizedRequest] = "Server return status: UnauthorizedRequest";
  CrtLogMap[DatabaseConnectionError] =
      "Server return status: DatabaseConnectionError";
  CrtLogMap[DatabaseTransactionError] =
      "Server return status: DatabaseTransactionError";
  CrtLogMap[FirmwareGeneratorInitError] =
      "Server return status: FirmwareGeneratorInitError";
  CrtLogMap[StickerPrinterInitError] =
      "Server return status: StickerPrinterInitError";
  CrtLogMap[RecordMissed] = "Server return status: RecordMissed";
  CrtLogMap[ProductionLineMissed] =
      "Server return status: ProductionLineMissed";
  CrtLogMap[TranspoderMissed] = "Server return status: TranspoderMissed";
  CrtLogMap[BoxMissed] = "Server return status: BoxMissed";
  CrtLogMap[PalletMissed] = "Server return status: PalletMissed";
  CrtLogMap[OrderMissed] = "Server return status: OrderMissed";
  CrtLogMap[MasterKeysMissed] = "Server return status: MasterKeysMissed";
  CrtLogMap[TransponderNotReleasedEarlier] =
      "Server return status: TransponderNotReleasedEarlier";
  CrtLogMap[TransponderNotAwaitingConfirmation] =
      "Server return status: TransponderNotAwaitingConfirmation";
  CrtLogMap[TransponderIncorrectRerelease] =
      "Server return status: TransponderIncorrectRerelease";
  CrtLogMap[IssuerMissed] = "Server return status: IssuerMissed";
  CrtLogMap[IdenticalUcidError] = "Server return status: IdenticalUcidError";
  CrtLogMap[CurrentOrderAssembled] =
      "Server return status: CurrentOrderAssembled";
  CrtLogMap[ProductionLineLaunchError] =
      "Server return status: ProductionLineLaunchError";
  CrtLogMap[ProductionLineAlreadyLaunched] =
      "Server return status: ProductionLineAlreadyLaunched";
  CrtLogMap[ProductionLineNotActive] =
      "Server return status: ProductionLineNotActive";
  CrtLogMap[ProductionLineNotInProcess] =
      "Server return status: ProductionLineNotInProcess";
  CrtLogMap[ProductionLineCompleted] =
      "Server return status: ProductionLineCompleted";
  CrtLogMap[ProductionLineNotInProcess] =
      "Server return status: ProductionLineNotInProcess";
  CrtLogMap[ProductionLineShutdownError] =
      "Server return status: ProductionLineShutdownError";
  CrtLogMap[ProductionLineRollbackLimit] =
      "Server return status: ProductionLineRollbackLimit";
  CrtLogMap[OrderInProcessMissed] =
      "Server return status: OrderInProcessMissed";
  CrtLogMap[FreeBoxMissed] = "Server return status: FreeBoxMissed";
  CrtLogMap[FirmwareGenerationError] =
      "Server return status: FirmwareGenerationError";
  CrtLogMap[PrinterConnectionError] =
      "Server return status: PrinterConnectionError";
  CrtLogMap[PrinterLibraryError] = "Server return status: PrinterLibraryError";
  CrtLogMap[BoxStickerPrintError] =
      "Server return status: BoxStickerPrintError";
  CrtLogMap[BoxStickerPrintError] =
      "Server return status: BoxStickerPrintError";
  CrtLogMap[PalletStickerPrintError] =
      "Server return status: PalletStickerPrintError";
  CrtLogMap[Unknown] = "Server return status: Unknown";
}
