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
  InvalidFile,
  DynamicLibraryMissing,

  ServerConnectionError,
  ServerNotResponding,
  ServerDataTransmittingError,
  ServerResponseSyntaxError,
  ServerResponseDataBlockError,
  ServerResponseProcessingError,

  ClientCommandParamError,

  ServerInternalError,

  OrderMultiplyAssembly,
  OrderInProcessMissed,

  ProductionLineContextNotAuthorized,
  ProductionLineNotLaunched,
  ProductionLineMissed,
  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineRollbackLimit,
  ProductionLineNotActive,
  ProductionLineNotInProcess,
  ProductionLineCompleted,

  BoxCompletelyAssembled,
  BoxNotCompletelyAssembled,

  IdenticalUcidError,
  TransponderIncorrectRerelease,
  CurrentOrderAssembled,
  FreeBoxMissed,

  StickerPrinterInitError,
  StickerPrinterConnectionError,

  ProgrammatorError,
};

#endif  // TYPES_H
