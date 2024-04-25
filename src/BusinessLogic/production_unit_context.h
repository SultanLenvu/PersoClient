#ifndef PRODUCTIONUNITCONTEXT_H
#define PRODUCTIONUNITCONTEXT_H

#include <QObject>

#include "types.h"

class ProductionUnitContext final : public QObject {
  Q_OBJECT

 private:
  StringDictionary State;
  StringDictionary Box;
  StringDictionary Transponder;
  QByteArray Firmware;

 public:
  explicit ProductionUnitContext() = default;
  ~ProductionUnitContext() = default;
  Q_DISABLE_COPY_MOVE(ProductionUnitContext)

 public:
  void setState(const StringDictionary& state);
  void setBox(const StringDictionary& box);
  void setTransponder(const StringDictionary& transponder);
  void setFirmware(const QByteArray& firmware);

 public:
  StringDictionary state();
  StringDictionary box();
  StringDictionary transponder();
  QByteArray firmware();

 public:
  bool isBoxEmpty(void) const;

 public:
  void completeBox(void);
  void refundBox(void);
  void clear(void);

 signals:
  void stateChanged(const StringDictionary& data);
  void boxChanged(const StringDictionary& data);
  void transponderChanged(const StringDictionary& data);
  void firmwareChanged(const QByteArray& firmware);
};

#endif  // PRODUCTIONUNITCONTEXT_H
