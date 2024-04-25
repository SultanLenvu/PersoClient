#ifndef TYPES_H
#define TYPES_H

#include <QHash>

using StringDictionary = QHash<QString, QString>;

enum class ReturnStatus {
  NoError = 0,
  ParameterError,
  SyntaxError,
  ConsistencyViolation,
  FileOpenError,
  FileWriteError,

  ServerConnectionError,
  ServerConnectionMissed,
  ServerNotResponding,
  ServerDataTransmittingError,
  ServerResponseSyntaxError,
  ServerResponseDataBlockError,
  ServerResponseProcessingError,

  ClientCommandParamError,
  ServerInternalError,

  ProductionContextNotValid,

  ProductionLineMissed,
  TransponderMissed,
  BoxMissed,
  PalletMissed,
  OrderMissed,
  IssuerMissed,
  MasterKeysMissed,

  OrderMultiplyAssembly,
  OrderInProcessMissed,

  BoxAlreadyRequested,
  FreeBoxMissed,
  BoxIsEmty,
  BoxOverflow,
  BoxCompletelyAssembled,
  BoxNotCompletelyAssembled,

  PalletIsEmpty,
  PalletOverflow,

  TransponderIncorrectRerelease,
  TransponderIdenticalUcidError,
  OrderCompletelyAssembled,
  TransponderRollbackLimit,

  ProductionLineContextNotAuthorized,
  ProductionLineNotLaunched,
  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineAlreadyInProcess,
  ProductionLineNotActive,
  ProductionLineCompleted,
  ProductionLineNotInProcess,

  StickerPrintingFailed,
  StickerPrinterDriverMissed,
  StickerPrinterLibraryMissing,
  StickerPrinterInitError,
  StickerPrinterConnectionError,
  StickerPrinterCommandScriptExecutionError,
  LastStickerMissed,

  InvalidFirmwareFile,
  ProgrammatorLibraryMissing,
  ProgrammatorExeFileInvalid,
  ProgrammatorCommandScriptError,

  Unknown
};

#endif  // TYPES_H
