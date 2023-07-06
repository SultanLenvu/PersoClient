#include "jlink_manual_programmer.h"

JLinkManualProgrammer::JLinkManualProgrammer(QObject *parent)
    : InterfaceProgrammer(parent) {}

void JLinkManualProgrammer::loadFirmware() {
  if (FirmwareFile == nullptr) {
    emit logging(
        QString("The firmware file is missing in the default directory, "
                "select it manually."));
    return;
  }

  emit logging(QString("Program firmware."));
}

void JLinkManualProgrammer::eraseFirmware() {
  if (FirmwareFile == nullptr) {
    emit logging(
        QString("The firmware file is missing in the default directory, "
                "select it manually."));
    return;
  }

  emit logging(QString("Erase firmware."));
}
