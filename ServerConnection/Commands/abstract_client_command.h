#ifndef ABSTRACTCLIENTCOMMAND_H
#define ABSTRACTCLIENTCOMMAND_H

#include <QJsonObject>
#include <QObject>

#include "types.h"

class AbstractClientCommand : public QObject {
  Q_OBJECT

  enum CommandReturnStatus {
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

    OrderMultiplyAssembly,
    OrderAssemblyMissing,

    BoxCompletelyAssembled,
    BoxNotCompletelyAssembled,

    TransponderNotReleasedEarlier,
    TransponderNotAwaitingConfirmation,
    TransponderIncorrectRerelease,
    IdenticalUcidError,
    CurrentOrderAssembled,

    ProductionLineLaunchError,
    ProductionLineAlreadyLaunched,
    ProductionLineAlreadyInProcess,
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

 protected:
  QJsonObject Request;
  QJsonObject Response;

  std::unordered_map<CommandReturnStatus, ReturnStatus> CrtMap;
  std::unordered_map<CommandReturnStatus, QString> CrtLogMap;

 public:
  explicit AbstractClientCommand(const QString& name);
  virtual ~AbstractClientCommand();

  virtual const QString& name(void) = 0;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) = 0;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) = 0;

  void clear(void);

 private:
  AbstractClientCommand();
  Q_DISABLE_COPY_MOVE(AbstractClientCommand)

 protected:
  void sendLog(const QString& log);
  void generateDataBlock(QByteArray& dataBlock);
  bool processDataBlock(const QByteArray& dataBlock);
  ReturnStatus processReturnStatus(const QString& ret);

 private:
  void createCrtMap(void);
  void createCrtLogMap(void);

 signals:
  void logging(const QString& log);
};

#endif  // ABSTRACTCLIENTCOMMAND_H
