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

  createCrtMap();
  createCrtLogMap();
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
    sendLog("Получена ошибка при обработке полученного блока данных.");
    return false;
  }

  // Выделяем список пар ключ-значение из JSON-файла
  Response = responseDocument.object();
  return true;
}

ReturnStatus AbstractClientCommand::processReturnStatus(const QString& ret) {
  CommandReturnStatus cret = static_cast<CommandReturnStatus>(ret.toInt());

  sendLog(CrtLogMap[cret]);

  return CrtMap[cret];
}

void AbstractClientCommand::createCrtMap() {
  CrtMap[NoError] = ReturnStatus::NoError;
  CrtMap[DynamicLibraryMissing] = ReturnStatus::ServerInternalError;
  CrtMap[ParameterError] = ReturnStatus::ServerInternalError;
  CrtMap[SyntaxError] = ReturnStatus::ServerInternalError;
  CrtMap[ConsistencyViolation] = ReturnStatus::ServerInternalError;
  CrtMap[FileOpenError] = ReturnStatus::ServerInternalError;

  CrtMap[DatabaseConnectionError] = ReturnStatus::ServerInternalError;
  CrtMap[DatabaseTransactionError] = ReturnStatus::ServerInternalError;
  CrtMap[DatabaseQueryError] = ReturnStatus::ServerInternalError;

  CrtMap[FirmwareGeneratorInitError] = ReturnStatus::ServerInternalError;
  CrtMap[StickerPrinterInitError] = ReturnStatus::ServerInternalError;

  CrtMap[RecordMissed] = ReturnStatus::ServerInternalError;
  CrtMap[ProductionLineMissed] = ReturnStatus::ProductionLineMissed;
  CrtMap[TranspoderMissed] = ReturnStatus::TranspoderMissed;
  CrtMap[BoxMissed] = ReturnStatus::BoxMissed;
  CrtMap[PalletMissed] = ReturnStatus::PalletMissed;
  CrtMap[OrderMissed] = ReturnStatus::OrderMissed;
  CrtMap[MasterKeysMissed] = ReturnStatus::MasterKeysMissed;

  CrtMap[OrderMultiplyAssembly] = ReturnStatus::OrderMultiplyAssembly;
  CrtMap[OrderInProcessMissed] = ReturnStatus::OrderInProcessMissed;

  CrtMap[BoxCompletelyAssembled] = ReturnStatus::BoxCompletelyAssembled;
  CrtMap[BoxNotCompletelyAssembled] = ReturnStatus::BoxNotCompletelyAssembled;

  CrtMap[TransponderNotReleasedEarlier] = ReturnStatus::ServerInternalError;
  CrtMap[TransponderNotAwaitingConfirmation] =
      ReturnStatus::ServerInternalError;
  CrtMap[TransponderIncorrectRerelease] =
      ReturnStatus::TransponderIncorrectRerelease;
  CrtMap[IdenticalUcidError] = ReturnStatus::IdenticalUcidError;
  CrtMap[CurrentOrderAssembled] = ReturnStatus::CurrentOrderAssembled;

  CrtMap[ProductionLineContextNotAuthorized] =
      ReturnStatus::ProductionLineContextNotAuthorized;
  CrtMap[ProductionLineNotLaunched] = ReturnStatus::ProductionLineNotLaunched;
  CrtMap[ProductionLineLaunchError] = ReturnStatus::ProductionLineLaunchError;
  CrtMap[ProductionLineAlreadyLaunched] =
      ReturnStatus::ProductionLineAlreadyLaunched;
  CrtMap[ProductionLineNotActive] = ReturnStatus::ProductionLineNotActive;
  CrtMap[ProductionLineCompleted] = ReturnStatus::ProductionLineCompleted;
  CrtMap[ProductionLineShutdownError] = ReturnStatus::ServerInternalError;

  CrtMap[ProductionLineNotInProcess] = ReturnStatus::ProductionLineNotInProcess;
  CrtMap[ProductionLineRollbackLimit] =
      ReturnStatus::ProductionLineRollbackLimit;
  CrtMap[FreeBoxMissed] = ReturnStatus::FreeBoxMissed;

  CrtMap[FirmwareGenerationError] = ReturnStatus::ServerInternalError;

  CrtMap[StickerPrinterConnectionError] = ReturnStatus::ServerInternalError;
  CrtMap[BoxStickerPrintError] = ReturnStatus::ServerInternalError;
  CrtMap[PalletStickerPrintError] = ReturnStatus::ServerInternalError;
  CrtMap[Unknown] = ReturnStatus::ServerInternalError;
}

void AbstractClientCommand::createCrtLogMap() {
  CrtLogMap[NoError] = "Server return status: NoError. ";
  CrtLogMap[DynamicLibraryMissing] =
      "Server return status: DynamicLibraryMissing. ";
  CrtLogMap[ParameterError] = "Server return status: ParameterError";
  CrtLogMap[SyntaxError] = "Server return status: SyntaxError";
  CrtLogMap[ConsistencyViolation] =
      "Server return status: ConsistencyViolation";
  CrtLogMap[FileOpenError] = "Server return status: FileOpenError";

  CrtLogMap[DatabaseConnectionError] =
      "Server return status: DatabaseConnectionError";
  CrtLogMap[DatabaseTransactionError] =
      "Server return status: DatabaseTransactionError";
  CrtLogMap[DatabaseQueryError] = "Server return status: DatabaseQueryError";

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

  CrtLogMap[OrderMultiplyAssembly] =
      "Server return status: OrderMultiplyAssembly";
  CrtLogMap[OrderInProcessMissed] =
      "Server return status: OrderAssemblyMissingы";

  CrtLogMap[BoxCompletelyAssembled] =
      "Server return status: BoxCompletelyAssembled";
  CrtLogMap[BoxNotCompletelyAssembled] =
      "Server return status: BoxNotCompletelyAssembled";

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

  CrtLogMap[ProductionLineContextNotAuthorized] =
      "Server return status: ProductionLineContextNotAuthorized";
  CrtLogMap[ProductionLineNotLaunched] =
      "Server return status: ProductionLineNotLaunched";
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
  CrtLogMap[ProductionLineShutdownError] =
      "Server return status: ProductionLineShutdownError";

  CrtLogMap[ProductionLineNotInProcess] =
      "Server return status: ProductionLineNotInProcess";
  CrtLogMap[ProductionLineRollbackLimit] =
      "Server return status: ProductionLineRollbackLimit";
  CrtLogMap[FreeBoxMissed] = "Server return status: FreeBoxMissed";

  CrtLogMap[FirmwareGenerationError] =
      "Server return status: FirmwareGenerationError";

  CrtLogMap[StickerPrinterConnectionError] =
      "Server return status: StickerPrinterConnectionError";
  CrtLogMap[BoxStickerPrintError] =
      "Server return status: BoxStickerPrintError";
  CrtLogMap[PalletStickerPrintError] =
      "Server return status: PalletStickerPrintError";
  CrtLogMap[Unknown] = "Server return status: Unknown";
}
