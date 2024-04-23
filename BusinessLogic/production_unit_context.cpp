#include "production_unit_context.h"

void ProductionUnitContext::setState(const StringDictionary& state) {
  State = state;
  emit stateChanged(state);
}

void ProductionUnitContext::setBox(const StringDictionary& box) {
  Box = box;
  emit boxChanged(Box);
}

void ProductionUnitContext::setTransponder(
    const StringDictionary& transponder) {
  Transponder = transponder;
  emit transponderChanged(Transponder);
}

void ProductionUnitContext::setFirmware(const QByteArray& firmware) {
  Firmware = firmware;
  emit firmwareChanged(Firmware);
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
  Transponder.clear();
  Firmware.clear();

  emit boxChanged(Box);
  emit transponderChanged(Transponder);
  emit firmwareChanged(Firmware);
}

void ProductionUnitContext::refundBox() {
  Box.clear();
  Transponder.clear();
  Firmware.clear();

  emit boxChanged(Box);
  emit transponderChanged(Transponder);
  emit firmwareChanged(Firmware);
}

void ProductionUnitContext::clear() {
  State.clear();
  Box.clear();
  Transponder.clear();
  Firmware.clear();

  emit stateChanged(State);
  emit boxChanged(Box);
  emit transponderChanged(Transponder);
  emit firmwareChanged(Firmware);
}
