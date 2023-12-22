#ifndef PALLETSTICKERPRINTCOMMAND_H
#define PALLETSTICKERPRINTCOMMAND_H

#include "abstract_client_command.h"
#include "definitions.h"

class PalletStickerPrintCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = COMMAND_PRINTPALLETSTICKER_NAME;
  const size_t ResponseSize = COMMAND_PRINTLASTPALLETSTICKER_RESPONSE_SIZE;

 public:
  explicit PalletStickerPrintCommand(const QString& name);
  ~PalletStickerPrintCommand();

  // AbstractClientCommand interface
 public:
  virtual const QString& name() override;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) override;

 private:
  Q_DISABLE_COPY_MOVE(PalletStickerPrintCommand)

 signals:
};

#endif  // PALLETSTICKERPRINTCOMMAND_H
