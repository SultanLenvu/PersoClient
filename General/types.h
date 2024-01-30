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
  BoxCompletelyAssembled,
  BoxNotCompletelyAssembled,

  PalletIsEmpty,

  TransponderIncorrectRerelease,
  IdenticalUcidError,
  CurrentOrderAssembled,
  TransponderRollbackLimit,

  ProductionLineContextNotAuthorized,
  ProductionLineNotLaunched,
  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineAlreadyInProcess,
  ProductionLineNotActive,
  ProductionLineCompleted,
  ProductionLineNotInProcess,

  StickerPrinterDriverMissed,
  StickerPrinterLibraryMissing,
  StickerPrinterInitError,
  StickerPrinterConnectionError,

  InvalidFirmwareFile,
  ProgrammatorLibraryMissing,
  ProgrammatorError,
};

#endif  // TYPES_H
