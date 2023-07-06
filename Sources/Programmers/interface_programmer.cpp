#include "interface_programmer.h"

InterfaceProgrammer::InterfaceProgrammer(QObject *parent) : QObject(parent) {
  processingFirmwarePath(DEFAULT_FIRMWARE_FILE_PATH);
}

void InterfaceProgrammer::setFirmwareFile(const QString &path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(QString("The firmware file is missing in the default "
                         "directory, select it manually."));
}

void InterfaceProgrammer::processingFirmwarePath(const QString &path) {
  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path);
  else
    FirmwareFile = nullptr;
}
