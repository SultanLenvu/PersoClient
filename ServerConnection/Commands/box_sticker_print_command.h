#ifndef BOXSTICKERPRINTCOMMAND_H
#define BOXSTICKERPRINTCOMMAND_H

#include "abstract_client_command.h"

class BoxStickerPrintCommand : public AbstractClientCommand {
  Q_OBJECT
 private:
  const QString Name = "box_sticker_print";
  const size_t Size = 2;

  StringDictionary Parameters;
  ReturnStatus Status;

 public:
  explicit BoxStickerPrintCommand(const QString& name);
  ~BoxStickerPrintCommand();

  // AbstractClientCommand interface
 public:
  virtual const QString& name() override;
  virtual ReturnStatus generate(QByteArray& dataBlock) override;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock) override;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) override;
  virtual void clear() override;

 private:
  Q_DISABLE_COPY_MOVE(BoxStickerPrintCommand)

 signals:
  void printBoxSticker_signal(const StringDictionary& data,
                              ReturnStatus& status);
};

#endif  // BOXSTICKERPRINTCOMMAND_H
