#include "interface_programmer.h"

InterfaceProgrammer::InterfaceProgrammer(QObject *parent, ProgrammerType type)
    : QObject(parent) {
  Type = type;
}

InterfaceProgrammer::~InterfaceProgrammer() {}

InterfaceProgrammer::ProgrammerType InterfaceProgrammer::type() const {
  return Type;
}

bool InterfaceProgrammer::checkFirmwareFile(QFile* firmware) {
  if (firmware == nullptr)
    return false;

  QFileInfo info(*firmware);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проврека на размер
  if ((info.size() > FIRMWARE_FILE_MAX_SIZE) || (info.size() == 0))
    return false;

  return true;
}

bool InterfaceProgrammer::checkDataFile(QFile* data) {
  if (data == nullptr)
    return false;

  QFileInfo info(*data);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проверка на размер
  if ((info.size() > USER_DATA_FLASH_SIZE) || (info.size() == 0))
    return false;

  return true;
}
