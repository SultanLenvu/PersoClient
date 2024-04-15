#include "production_unit_context.h"

ProductionUnitContext::ProductionUnitContext(const QString& name)
    : NamedObject(name) {}

void ProductionUnitContext::setState(const StringDictionary&& state) {
  State = state;
  emit stateChanged(state);
}

void ProductionUnitContext::setBox(const StringDictionary&& box) {
  Box = box;
  emit boxChanged(Box);
}

void ProductionUnitContext::setTransponder(
    const StringDictionary&& transponder) {
  Transponder = transponder;
  emit transponderChanged(Transponder);
}

void ProductionUnitContext::setFirmware(const QByteArray&& firmware) {
  Firmware = firmware;
}

StringDictionary ProductionUnitContext::state() {
  return State;
}

StringDictionary ProductionUnitContext::box() {
  return Box;
}

StringDictionary ProductionUnitContext::transponder() {
  return Transponder;
}

QByteArray ProductionUnitContext::firmware() {
  return Firmware;
}

bool ProductionUnitContext::isBoxEmpty() const {
  return Box.value("box_assembled_units") == "0";
}

void ProductionUnitContext::completeBox() {
  Box.clear();
  emit boxChanged(Box);
}

void ProductionUnitContext::refundBox() {
  Box.clear();
  emit boxChanged(Box);
}

void ProductionUnitContext::clear() {
  State.clear();
  Box.clear();
  Transponder.clear();

  emit stateChanged(State);
  emit boxChanged(Box);
  emit transponderChanged(Transponder);
}
