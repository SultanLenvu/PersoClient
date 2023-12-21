#ifndef TYPES_H
#define TYPES_H

#include <QHash>

using StringDictionary = QHash<QString, QString>;

enum class ReturnStatus {
  NoError = 0,
  ParameterError,
  SyntaxError,
  SynchronizationError,
  FileOpenError,
  InvalidFile,
  DynamicLibraryMissing,

  ServerConnectionError,
  ServerNotResponding,
  ServerDataTransmittingError,
  ServerResponseSyntaxError,
  ServerResponseDataBlockError,
  ServerResponseProcessingError,
  ServerInternalError,
  ClientCommandParamError,

  StickerPrinterInitError,
  StickerPrinterConnectionError,

  ProgrammatorError,
};

enum class ServerReturnStatus {
  NoError = 0,
  ParameterError,
  SyntaxError,
  SynchronizationError,
  FileOpenError,
  InvalidProductionLineContext,
  UnauthorizedRequest,

  DatabaseConnectionError,
  DatabaseTransactionError,
  DatabaseQueryError,

  FirmwareGeneratorInitError,
  StickerPrinterInitError,

  RecordMissed,
  ProductionLineMissed,
  TranspoderMissed,
  BoxMissed,
  PalletMissed,
  OrderMissed,
  IssuerMissed,
  MasterKeysMissed,

  TransponderNotReleasedEarlier,
  TransponderNotAwaitingConfirmation,
  TransponderIncorrectRerelease,
  IdenticalUcidError,
  CurrentOrderAssembled,

  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineNotActive,
  ProductionLineNotInProcess,
  ProductionLineCompleted,
  ProductionLineShutdownError,
  ProductionLineRollbackLimit,
  OrderInProcessMissed,
  FreeBoxMissed,

  FirmwareGenerationError,

  PrinterConnectionError,
  PrinterLibraryError,
  BoxStickerPrintError,
  PalletStickerPrintError,
  Unknown,
};

#endif  // TYPES_H
