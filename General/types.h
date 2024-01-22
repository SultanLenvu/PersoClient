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

  ClientCommandParamError,

  ServerInternalError,

  OrderMultiplyAssembly,
  OrderAssemblyMissing,

  BoxCompletelyAssembled,
  BoxNotCompletelyAssembled,

  ProductionLineMissed,
  ProductionLineLaunchError,
  ProductionLineAlreadyLaunched,
  ProductionLineRollbackLimit,
  ProductionLineNotActive,
  ProductionLineNotInProcess,
  ProductionLineCompleted,
  OrderInProcessMissed,
  IdenticalUcidError,
  TransponderIncorrectRerelease,
  CurrentOrderAssembled,
  FreeBoxMissed,

  StickerPrinterInitError,
  StickerPrinterConnectionError,

  ProgrammatorError,
};

#endif  // TYPES_H
