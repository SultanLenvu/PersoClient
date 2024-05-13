#include "abstract_programmer.h"
#include "definitions.h"

AbstractProgrammer::AbstractProgrammer(const QString& name) : QObject(nullptr) {
  setObjectName(name);
}

AbstractProgrammer::~AbstractProgrammer() {}

bool AbstractProgrammer::checkFirmwareFile(const QFile& firmware) {
  QFileInfo info(firmware);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проврека на размер
  if (info.size() != FIRMWARE_SIZE)
    return false;

  return true;
}

bool AbstractProgrammer::checkDataFile(const QFile& data) {
  QFileInfo info(data);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проверка на размер
  if ((info.size() > USER_DATA_FLASH_SIZE) || (info.size() == 0))
    return false;

  return true;
}
