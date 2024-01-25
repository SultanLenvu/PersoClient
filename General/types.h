#ifndef TYPES_H
#define TYPES_H

#include <QHash>

using StringDictionary = QHash<QString, QString>;

enum class ReturnStatus {
  NoError = 0,
  DynamicLibraryMissing,
  ParameterError,
  SyntaxError,
  ConsistencyViolation,
  FileOpenError,
  InvalidFile,

  ServerConnectionError,
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
  MasterKeysMissed,

  OrderMultiplyAssembly,
  OrderInProcessMissed,

  FreeBoxMissed,
  BoxCompletelyAssembled,
  BoxNotCompletelyAssembled,

  TransponderIncorrectRerelease,
  IdenticalUcidError,
  CurrentOrderAssembled,

  ProductionLineContextNotAuthorized,
  ProductionLineNotLaunched,
  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineNotActive,
  ProductionLineCompleted,
  ProductionLineNotInProcess,
  ProductionLineRollbackLimit,

  StickerPrinterInitError,
  StickerPrinterConnectionError,

  ProgrammatorError,
};

#endif  // TYPES_H
