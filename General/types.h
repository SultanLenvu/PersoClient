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

  ServerConnectionError,
  ServerConnectionMissed,
  ServerNotResponding,
  ServerDataTransmittingError,
  ServerResponseSyntaxError,
  ServerResponseDataBlockError,
  ServerResponseProcessingError,

  ClientCommandParamError,
  ServerInternalError,

  ProductionLineMissed,
  TranspoderMissed,
  BoxMissed,
  PalletMissed,
  OrderMissed,
  IssuerMissed,
  MasterKeysMissed,

  OrderMultiplyAssembly,
  OrderInProcessMissed,

  BoxAlreadyRequested,
  BoxNotRequested,
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
  StickerPrintError,
  LastStickerMissed,

  InvalidFirmwareFile,
  ProgrammatorLibraryMissing,
  ProgrammatorExeFileInvalid,
  ProgrammatorCommandScriptError,

  Unknown
};

#endif  // TYPES_H
